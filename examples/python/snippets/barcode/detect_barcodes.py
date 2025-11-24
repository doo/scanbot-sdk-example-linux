from scanbotsdk import *
from typing import List


def scan_barcode(image: ImageRef):
    configuration = BarcodeScannerConfiguration(
        return_barcode_image=True
    )
    configuration.barcode_format_configurations = [
        BarcodeFormatCommonConfiguration(
            gs_1_handling=Gs1Handling.DECODE_FULL,
        )
    ]

    scanner = BarcodeScanner(configuration=configuration)
    result: BarcodeScannerResult = scanner.run(image=image)

    print_barcodes(result.barcodes)


def print_barcodes(barcodes: List[BarcodeItem]):
    if not barcodes:
        print("No barcodes found")
        return

    print(f"Found {len(barcodes)} barcodes:")
    for i, barcode in enumerate(barcodes, start=1):
        print(f"Barcode #{i}:")
        print(f"\tFormat: {barcode.format.name}")
        print(f"\tText:   {barcode.text}")
