from scanbotsdk import *

def save_image(image: ImageRef, path: str) -> None:
    options = SaveImageOptions(quality=80)
    image.save_image(path, options)
    
def rotate_90(image: ImageRef) -> None:
    processor = ImageProcessor()
    rotated = processor.rotate(image=image, rotation=ImageRotation.CLOCKWISE_90)

    original_info = image.info()
    rotated_info = rotated.info()

    print(f"Original WxH: {original_info.width}x{original_info.height}")
    print(f"Rotated 90° WxH: {rotated_info.width}x{rotated_info.height}")


def rotate_180(image: ImageRef) -> None:
    processor = ImageProcessor()
    rotated = processor.rotate(image=image, rotation=ImageRotation.CLOCKWISE_180)

    rotated_info = rotated.info()
    print(f"Rotated 180° WxH: {rotated_info.width}x{rotated_info.height}")


def resize(image: ImageRef, max_size: int) -> None:
    processor = ImageProcessor()
    resized = processor.resize(image=image, size=max_size)

    resized_info = resized.info()
    print(f"Resized WxH: {resized_info.width}x{resized_info.height}")


def apply_binarization_filter(image: ImageRef, path: str) -> None:
    processor = ImageProcessor()
    image_filter = CustomBinarizationFilter(preset=BinarizationFilterPreset.PRESET_4)
    filtered_img = processor.apply_filter(image=image, filter=image_filter)
    filtered_img.save_image(path)


def detect_and_crop_document(image: ImageRef) -> None:
    processor = ImageProcessor()
    scanner = DocumentScanner(configuration=DocumentScannerConfiguration())

    detection_result = scanner.run(image=image)
    print(f"Detection status: {detection_result.status}")
    print(f"Detected points: {len(detection_result.points)}")

    cropped = processor.crop(image=image, quad=detection_result.points_normalized)
    cropped_info = cropped.info()
    print(f"Cropped WxH: {cropped_info.width}x{cropped_info.height}")
