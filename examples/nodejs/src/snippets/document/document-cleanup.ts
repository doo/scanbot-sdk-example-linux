import * as ScanbotSDK from "scanbotsdk";

export class DocumentCleanupSnippet {
  public static async run(filePath: string, maskPath: string, savePath?: string): Promise<void> {
    await using image = await ScanbotSDK.ImageRef.fromPath(filePath);
    await using mask = await ScanbotSDK.ImageRef.fromPath(maskPath);
    await using cleanup = await ScanbotSDK.DocumentCleanup.create(
      image,
      new ScanbotSDK.DocumentCleanupConfiguration()
    );
    await using result = await cleanup.run(mask);

    console.log("Document cleanup status: " + result.status);

    const originalInfo = await image.imageInfo();
    const cleanedInfo = await result.image.imageInfo();

    console.log(`Original WxH: ${originalInfo.width}x${originalInfo.height}`);
    console.log(`Cleaned  WxH: ${cleanedInfo.width}x${cleanedInfo.height}`);

    if (savePath) {
      await result.image.saveImage(savePath, new ScanbotSDK.SaveImageOptions());
      console.log("Saved cleaned image to: " + savePath);
    }
  }
}
