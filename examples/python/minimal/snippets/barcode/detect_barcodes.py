from scanbotsdk import *

def scan_barcode(image: ImageRef):
    configuration = BarcodeScannerConfiguration(
        return_barcode_image=True
    )
    configuration.barcode_format_configurations = [BarcodeFormatCommonConfiguration(
        gs_1_handling = Gs1Handling.DECODE_FULL,
    )]
    # Configure other parameters as needed.
    
    scanner = BarcodeScanner(configuration=configuration)
    result: BarcodeScannerResult = scanner.run(image=image)

    if result.barcodes:
        print(f"Found {len(result.barcodes)} barcodes:")
        for barcode in result.barcodes:
            print(f"\t{barcode.text}")
    else:
        print("No barcodes found")
