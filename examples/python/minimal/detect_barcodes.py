import sys
import scanbotsdk

LICENSE_KEY:str = "Put-your-license-key-here"

# Setup and initialize the Scanbot SDK
print(f"Initializing Scanbot SDK...")
scanbotsdk.initialize(LICENSE_KEY)
print(f"License Status: {scanbotsdk.get_license_status()}")

# Load the input image
input_path = sys.argv[1]

image = scanbotsdk.ImageRef.from_path(input_path)
# Setup scanning configuration
configuration = scanbotsdk.BarcodeScannerConfiguration()

barcode_scanner = scanbotsdk.BarcodeScanner(configuration=configuration)
barcode_scanning_result = barcode_scanner.run(image=image)
barcodes = barcode_scanning_result.barcodes
if len(barcodes) > 0:
    print(f"Found {len(barcodes)} barcodes")
    for barcode in barcodes:
        print(f"\t{barcode.text}")
else:
    print(f"No barcodes found")
