from scanbotsdk import *
from utils import print_generic_document

def scan_check(image: ImageRef):
    config = CheckScannerConfiguration(
        document_detection_mode = CheckDocumentDetectionMode.DETECT_AND_CROP_DOCUMENT
    )
    # Configure other parameters as needed.
    
    scanner = CheckScanner(configuration=config)
    result: CheckScanningResult = scanner.run(image=image)

    detection_result = result.document_detection_result
    if detection_result is not None:
        print(f"Document Detection status result: {detection_result.status.value}")

    print(f"MagneticInkStripScanning Status: {result.status.name}")
    print_generic_document(result.check)