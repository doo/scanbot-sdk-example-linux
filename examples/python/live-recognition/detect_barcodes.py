import scanbotsdk
import argparse
from typing import Sequence
import cv2 as cv
import numpy as np
import os

LICENSE_KEY: str = "Put-your-license-key-here"

DEVICE_CAPTURE_WIDTH = 1280
DEVICE_CAPTURE_HEIGHT = 720
DEVICE_CAPTURE_FRAMERATE = 30


def gstreamer_pipeline(device):
    if device == "jetson_csi":
        ret = "nvarguscamerasrc ! "
        ret += f"video/x-raw(memory:NVMM), width=(int){DEVICE_CAPTURE_WIDTH}, height=(int){DEVICE_CAPTURE_HEIGHT}, "
        ret += f"framerate=(fraction){DEVICE_CAPTURE_FRAMERATE}/1 ! nvvidconv flip-method=0 ! "
        ret += f"video/x-raw, format=(string)BGRx ! "
        ret += "videoconvert ! video/x-raw, format=(string)BGR ! appsink drop=1"
    elif device == "libcamera":
        ret = "libcamerasrc ! video/x-raw, format=BGR, "
        ret += f" width=(int){DEVICE_CAPTURE_WIDTH},"
        ret += f" height=(int){DEVICE_CAPTURE_HEIGHT},"
        ret += f" framerate=(fraction){DEVICE_CAPTURE_FRAMERATE}/1 "
        ret += " ! appsink"
    else:
        raise ValueError("Invalid device name: " + device)

    return ret


def parse_barcodes(barcodes: Sequence[scanbotsdk.BarcodeItem], image):
    print('Recognized barcodes:')
    for barcode in barcodes:
        # remove all white-spaces, newlines and tabs from the barcode text
        text = barcode.text.replace(' ', '').replace('\n', '').replace('\t', '')
        print(('\t==> ' + text).encode('utf-8'))

        # draw the ROI with individual line
        quad = barcode.quad
        cv.line(image, (quad[0].x, quad[0].y), (quad[1].x, quad[1].y), (0, 255, 0), 2)
        cv.line(image, (quad[1].x, quad[1].y), (quad[2].x, quad[2].y), (0, 255, 0), 2)
        cv.line(image, (quad[2].x, quad[2].y), (quad[3].x, quad[3].y), (0, 255, 0), 2)
        cv.line(image, (quad[3].x, quad[3].y), (quad[0].x, quad[0].y), (0, 255, 0), 2)


def scan_live(scanner: scanbotsdk.BarcodeScanner, device: str, use_display: bool):
    DISPLAY_HEIGHT = 640

    try:
        cap = cv.VideoCapture(int(device), cv.CAP_V4L2)
        cap.set(cv.CAP_PROP_FRAME_WIDTH, DEVICE_CAPTURE_WIDTH)
        cap.set(cv.CAP_PROP_FRAME_HEIGHT, DEVICE_CAPTURE_HEIGHT)
        cap.set(cv.CAP_PROP_FPS, DEVICE_CAPTURE_FRAMERATE)
    except ValueError:
        cap = cv.VideoCapture(gstreamer_pipeline(device), cv.CAP_GSTREAMER)

    if not cap.isOpened():
        print("Cannot open camera")
        exit(1)

    try:
        if use_display:
            cv.namedWindow("BarcodeRecognizer", cv.WINDOW_AUTOSIZE)
        image = np.zeros((DEVICE_CAPTURE_HEIGHT, DEVICE_CAPTURE_WIDTH, 3), dtype=np.uint8)
        while True:
            # Note that the image is updated in-place
            ret, _ = cap.read(image)
            if not ret:
                print("Failed to capture image")
                break

            # Although it is not required to use the image ref as a context manager, it is recommended to do so when
            # creating from live source to ensure that the image_ref is timely released.
            #
            # Setting live_source=True lets the scanner know that we're running in live mode.
            # In this mode we maintain the highest FPS because we spread the work of scanning barcodes across multiple frames.
            # If you set live_source=False, the scanner will run in single-shot mode which is much slower,
            # but has a much higher probability of finding all barcodes in the input image.
            # As an alternative, you can explicitly set the scanner mode,
            # by creating it with e.g. processing_mode=ProcessingMode.LIVE for live mode.
            with scanbotsdk.ImageRef.from_ndarray(image, scanbotsdk.RawImageLoadOptions(live_source=True)) as image_ref:
                barcode_scanning_result = scanner.run(
                    image=image_ref)
                parse_barcodes(barcode_scanning_result.barcodes, image)

                if use_display:
                    width = image.shape[1] * DISPLAY_HEIGHT / image.shape[0]
                    to_show = cv.resize(image, (int(width), DISPLAY_HEIGHT))
                    cv.imshow("BarcodeRecognizer",
                              to_show)
                    cv.waitKey(1)

                    if cv.getWindowProperty("BarcodeRecognizer", cv.WND_PROP_AUTOSIZE) == -1:
                        break
    finally:
        cap.release()


def recognize():
    parser = argparse.ArgumentParser(description='Detect barcodes in an image.')
    parser.add_argument('--input', type=str, required=True,
                        help='Input device name (jetson_csi|libcamera) or number of /dev/video* device')
    parser.add_argument('--use-display', action='store_true', help='Show results in a window')
    parser.add_argument('--use-tensorrt', action='store_true', help='Use TensorRT backend for GPU acceleration')

    args = parser.parse_args()
    input_device = args.input
    use_display = args.use_display

    # Setup and initialize the Scanbot SDK
    print(f"Initializing Scanbot SDK...")
    scanbotsdk.initialize(LICENSE_KEY)
    scanbotsdk.wait_for_online_license_check_completion(5000)

    license_info = scanbotsdk.get_license_info()
    print(f"License Status: {license_info.status}")
    is_floating_license = license_info.devices is not None
    if is_floating_license:
        print(
            f"Using floating license with {license_info.devices} devices. Do not forget to call "
            f"scanbotsdk.deregister_device and scanbotsdk.wait_for_device_deregistration_completion when you no "
            f"longer need the license or use scanbotsdk.DeviceSession context manager.")

    # If you are not using floating license, it is not required to use scanbotsdk.DeviceSession context manager as there is no
    # need to notify server you are no longer using the license. Alternatively, you can manually call
    # scanbotsdk.deregister_device and scanbotsdk.wait_for_device_deregistration_completion if you need asynchronous
    # deregistration behaviour
    with scanbotsdk.DeviceSession(deregister_timeout_ms=15000):
        # Setup scanning configuration
        configuration = scanbotsdk.BarcodeScannerConfiguration(
            processing_mode=scanbotsdk.ProcessingMode.AUTO,
            barcode_format_configurations=[
                scanbotsdk.BarcodeFormatCommonConfiguration(formats=scanbotsdk.BarcodeFormats.common)])
        if args.use_tensorrt:
            configuration.accelerator = scanbotsdk.TensorRtAccelerator(engine_path=os.curdir)

        scanner = scanbotsdk.BarcodeScanner(configuration=configuration)

        scan_live(scanner, input_device, use_display)


if __name__ == '__main__':
    recognize()
