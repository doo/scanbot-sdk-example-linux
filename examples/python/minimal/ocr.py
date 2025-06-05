import sys
from timeit import default_timer as timer

import numpy as np
import cv2 as cv
import scanbotsdk

LICENSE_KEY:str = "Put-your-license-key-here"

if len(sys.argv) < 2:
    print("Usage: python ocr.py <input_image_path>")
    sys.exit(1)

# Setup and initialize the Scanbot SDK
print(f"Initializing Scanbot SDK...")
scanbotsdk.initialize(LICENSE_KEY)
scanbotsdk.wait_for_online_license_check_completion(15000)
license_info = scanbotsdk.get_license_info()
print(f"License Status: {license_info.status}")
is_floating_license = license_info.devices is not None
if is_floating_license:
    print(
        f"Using floating license with a limit of {license_info.devices} devices.")

# If you are not using floating license, it is not required to use scanbotsdk.DeviceSession context manager as there
# is no need to notify server you are no longer using the license. Alternatively, you can manually call
# scanbotsdk.deregister_device and scanbotsdk.wait_for_device_deregistration_completion if you need asynchronous
# deregistration behaviour
with scanbotsdk.DeviceSession(deregister_timeout_ms=15000):
    # Load the input image
    input_path = sys.argv[1]

    npimage = cv.imread(input_path)

    image = scanbotsdk.ImageRef.from_ndarray(npimage, scanbotsdk.RawImageLoadOptions(live_source=True))

    engine = scanbotsdk.OcrEngine()
    start_time = timer()
    page = engine.run(image=image)
    end_time = timer()
    print(f"OCR processing time: {1000 * (end_time - start_time):.2f} ms")

    for block in page.blocks:
        print(f'Block: "{block.text}"')
        for line in block.lines:
            print(f'  Line: "{line.text}"')
            for word in line.words:
                print(f'    Word: "{word.text}" (Confidence: {word.confidence})')

