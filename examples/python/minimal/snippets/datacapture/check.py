from scanbotsdk import *
from utils import print_generic_document

def scan_check(image: ImageRef):
    # Make sure you have a valid license
    license_info = get_license_info()
    if license_info.status != LicenseStatus.OKAY:
        return
    
    config = CheckScannerConfiguration(
        document_detection_mode = CheckDocumentDetectionMode.DETECT_AND_CROP_DOCUMENT
    )
    # Configure other parameters as needed.
    
    scanner = CheckScanner(configuration=config)
    result: CheckScanningResult = scanner.run(image=image)

    print(f"Document Detection status result: {result.document_detection_result.status.name}")
    print(f"Ink Status: {result.status.name}")
    print_generic_document(result.check)

    # if you want to use cropped image later, call result.cropped_image.encode(), and save buffer,
    # otherwise image reference will be released when the ImageRef object is garbage collected.
