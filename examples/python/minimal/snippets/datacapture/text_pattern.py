from scanbotsdk import *

def scan_text_pattern(image: ImageRef):
    # Make sure you have a valid license
    license_info = get_license_info()
    if license_info.status != LicenseStatus.OKAY:
        return
    
    config = TextPatternScannerConfiguration(
        minimum_number_of_required_frames_with_equal_scanning_result = 2,
        validator = DefaultContentValidator()
    )
    # Configure other parameters as needed.
    
    scanner = TextPatternScanner(configuration=config)
    result: TextPatternScannerResult = scanner.run(image=image)
    
    print(f"Raw Text: {result.raw_text}")

