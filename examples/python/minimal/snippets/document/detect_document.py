from scanbotsdk import *

def scan_document(image: ImageRef):
    # Make sure you have a valid license
    license_info = get_license_info()
    if license_info.status != LicenseStatus.OKAY:
        return
    
    config = DocumentScannerConfiguration()
    config.parameters.accepted_angle_score = 75
    config.parameters.ignore_orientation_mismatch = False
    # Configure other parameters as needed.
    
    scanner = DocumentScanner(configuration=config)
    result: DocumentDetectionResult = scanner.run(image=image)

    print(f"Detection status: {result.status.name}")
    if result.points:
        print("Detected points:")
        for point in result.points:
            print(f"\tx: {point.x}, y: {point.y}")
    else:
        print("No document corners detected.")
