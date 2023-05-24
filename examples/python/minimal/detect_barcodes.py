import sys
import cv2 as cv
import scanbotsdk

LICENSE_KEY:str = "Put-your-license-key-here"

# Setup and initialize the Scanbot SDK
print(f"Initializing Scanbot SDK...")
scanbotsdk.initialize(LICENSE_KEY)
print(f"License Status: {scanbotsdk.get_license_status()}")

# Load the input image
input = sys.argv[1]
image = cv.imread(input)
if image is None:
    print(f"Failed to load image: {input}")
    exit(1)

# Setup recognition parameters
params = scanbotsdk.BarcodeRecognizerInitParams(
    engine_mode=scanbotsdk.BarcodeRecognitionEngineMode.FAST,
)

## If you're running on an NVidia Jetson,
## uncomment the following line to use
## the TensorRT backend for GPU acceleration
# params.use_tensorrt = True

barcode_recognizer = scanbotsdk.BarcodeRecognizer(params)
barcodes = barcode_recognizer.recognize(image)
if len(barcodes) > 0:
    print(f"Found {len(barcodes)} barcodes")
    for barcode in barcodes:
        print(f"\t{barcode}")
else:
    print(f"No barcodes found")
