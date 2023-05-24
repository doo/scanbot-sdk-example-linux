import sys
import cv2 as cv
from scanbotsdk import *

LICENSE_KEY:str = "Put-your-license-key-here"

# Setup and initialize the Scanbot SDK
print(f"Initializing Scanbot SDK...")
sdk = ScanbotSDK(LICENSE_KEY)
print(f"License Staus: {sdk.get_license_status()}")


# The first arguments is expected to be the input image file path
input = sys.argv[1]
image = cv.imread(input)

# recognize barcodes
barcode_recognizer = BarcodeRecognizer(sdk, BarcodeRecognitionEngineMode.BALANCED)
barcodes = barcode_recognizer.recognize(image)
if len(barcodes) > 0:
    print(f"Found {len(barcodes)} barcodes")
    for barcode in barcodes:
        print(f"\t{barcode}")