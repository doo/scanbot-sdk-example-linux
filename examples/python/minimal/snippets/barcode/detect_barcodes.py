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
            # if you want to use image later, call barcode.source_image.encode(), and save buffer,
            # otherwise image reference will be released when the ImageRef object is garbage collected.
            print(f"\t{barcode.text}")
    else:
        print("No barcodes found")
