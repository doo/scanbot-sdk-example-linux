import scanbotsdk

def scan_barcode(image: scanbotsdk.ImageRef):
    configuration = scanbotsdk.BarcodeScannerConfiguration()
    scanner = scanbotsdk.BarcodeScanner(configuration=configuration)

    result = scanner.run(image=image)

    if result.barcodes:
        print(f"Found {len(result.barcodes)} barcodes:")
        for barcode in result.barcodes:
            print(f"\t{barcode.text}")
    else:
        print("No barcodes found")
