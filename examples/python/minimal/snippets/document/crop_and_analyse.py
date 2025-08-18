from scanbotsdk import *

from utils import create_image_ref

def crop_and_analyse(image_path: str):
    # Make sure you have a valid license
    license_info = get_license_info()
    if license_info.status != LicenseStatus.OKAY:
        return
    
    image: ImageRef = create_image_ref(image_path)
    
    config = DocumentScannerConfiguration()
    config.parameters.accepted_angle_score = 75
    config.parameters.ignore_orientation_mismatch = False
    # Configure other parameters as needed.
    
    scanner = DocumentScanner(configuration=config)
    result = scanner.scan(image=image)

    print(f"Detection status: {result.detection_result.status}")
    print(f"Total detection score: {result.detection_result.detection_scores.total_score}")

    if result.points:
        print("Detected points:")
        for p in result.points:
            print(f"\tx: {p.x}, y: {p.y}")
    else:
        print("No document corners detected.")

    analyser_config = DocumentQualityAnalyzerConfiguration(
        tile_size = 300,
        min_estimated_number_of_symbols_for_document = 20
    )
    # Configure other parameters as needed.
    
    analyser = DocumentQualityAnalyzer(configuration=analyser_config)
    quality_result: DocumentQualityAnalyzerResult = analyser.run(image=result.cropped_image)
    
    # if you want to use cropped image later, call result.cropped_image.encode(), and save buffer,
    # otherwise image reference will be released when the ImageRef object is garbage collected.

    print(f"Document Found: {quality_result.document_found}")
    print(f"Quality: {quality_result.quality.name}")