import * as ScanbotSDK from "scanbotsdk";


export async function rotate90(image: ScanbotSDK.ImageRef): Promise<void> {
  await using imageProcessor = await ScanbotSDK.ImageProcessor.create();
  await using rotated = await imageProcessor.rotate(image, "CLOCKWISE_90");

  const originalInfo = await image.imageInfo();
  const rotatedInfo = await rotated.imageInfo();

  console.log(`Original WxH: ${originalInfo.width}x${originalInfo.height}`);
  console.log(`Rotated 90° WxH: ${rotatedInfo.width}x${rotatedInfo.height}`);
}

export async function rotate180(image: ScanbotSDK.ImageRef): Promise<void> {
  await using imageProcessor = await ScanbotSDK.ImageProcessor.create();
  await using rotated = await imageProcessor.rotate(image, "CLOCKWISE_180");

  const rotatedInfo = await rotated.imageInfo();
  console.log(`Rotated 180° WxH: ${rotatedInfo.width}x${rotatedInfo.height}`);
}

export async function resize(image: ScanbotSDK.ImageRef, maxSize: number): Promise<void> {
  await using imageProcessor = await ScanbotSDK.ImageProcessor.create();
  await using resized = await imageProcessor.resize(image, maxSize);

  const resizedInfo = await resized.imageInfo();
  console.log(`Resized WxH: ${resizedInfo.width}x${resizedInfo.height}`);
}

export async function applyBinarizationFilter(image: ScanbotSDK.ImageRef): Promise<void> {
  await using imageProcessor = await ScanbotSDK.ImageProcessor.create();
  const filter = new ScanbotSDK.CustomBinarizationFilter({ preset: "PRESET_4" });
  await using filtered = await imageProcessor.applyFilter(image, filter);

  const info = await filtered.imageInfo();
  console.log(`Filtered WxH: ${info.width}x${info.height}`);
}

export async function detectAndCropDocument(image: ScanbotSDK.ImageRef): Promise<void> {
  await using imageProcessor = await ScanbotSDK.ImageProcessor.create();
  await using documentScanner = await ScanbotSDK.DocumentScanner.create();

  const detectionResult = await documentScanner.run(image);
  console.log(`Detection status: ${detectionResult.status}`);
  console.log(`Detected points: ${detectionResult.points.length}`);

  await using cropped = await imageProcessor.crop(image, detectionResult.pointsNormalized);
  const croppedInfo = await cropped.imageInfo();
  console.log(`Cropped WxH: ${croppedInfo.width}x${croppedInfo.height}`);
}

export async function processPipeline(image: ScanbotSDK.ImageRef) {
  await using imageProcessor = await ScanbotSDK.ImageProcessor.create();

  await using rotated = await imageProcessor.rotate(image, "CLOCKWISE_90");
  const originalInfo = await image.imageInfo();
  const rotatedInfo = await rotated.imageInfo();
  console.log(`Original WxH: ${originalInfo.width}x${originalInfo.height}`);
  console.log(`Rotated 90° WxH: ${rotatedInfo.width}x${rotatedInfo.height}`);

  const targetMax = Math.floor(Math.max(originalInfo.width, originalInfo.height) / 2);
  await using resized = await imageProcessor.resize(rotated, targetMax);
  const resizedInfo = await resized.imageInfo();
  console.log(`Resized WxH: ${resizedInfo.width}x${resizedInfo.height}`);

  await using documentScanner = await ScanbotSDK.DocumentScanner.create();
  const detectionResult = await documentScanner.run(resized);
  await using cropped = await imageProcessor.crop(resized, detectionResult.pointsNormalized);
  const croppedInfo = await cropped.imageInfo();
  console.log(`Cropped WxH: ${croppedInfo.width}x${croppedInfo.height}`);

  const filter = new ScanbotSDK.CustomBinarizationFilter({ preset: "PRESET_4" });
  const filtered = await imageProcessor.applyFilter(cropped, filter);
  const filteredInfo = await filtered.imageInfo();
  console.log(`Filtered WxH: ${filteredInfo.width}x${filteredInfo.height}`);

  return filtered;
}