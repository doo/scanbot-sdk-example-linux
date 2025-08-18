from scanbotsdk import *

def scan_vin(image: ImageRef):
    # Make sure you have a valid license
    license_info = get_license_info()
    if license_info.status != LicenseStatus.OKAY:
        return
    
    config = VinScannerConfiguration(
        extract_vin_from_barcode=True
    )
    # Configure other parameters as needed.
    
    scanner = VinScanner(configuration=config)
    result: VinScannerResult = scanner.run(image=image)
    
    print(f"Text: {result.text_result.raw_text}")
    print(f"Extracted VIN: {result.barcode_result.extracted_vin}")