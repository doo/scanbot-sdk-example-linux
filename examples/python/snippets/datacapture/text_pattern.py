from scanbotsdk import *

def scan_text_pattern(image: ImageRef):
    config = TextPatternScannerConfiguration(
        minimum_number_of_required_frames_with_equal_scanning_result = 1,
        validator = DefaultContentValidator()
    )
    # Configure other parameters as needed.
    
    scanner = TextPatternScanner(configuration=config)
    result: TextPatternScannerResult = scanner.run(image=image)
    
    print(f"Raw Text: {result.raw_text}")

