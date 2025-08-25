import os
import cv2 as cv
import numpy as np
from typing import Sequence
from scanbotsdk import *

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


def _parse_barcodes(barcodes: Sequence[BarcodeItem], image):
    print('Recognized barcodes:')
    for barcode in barcodes:
        text = barcode.text.replace(' ', '').replace('\n', '').replace('\t', '')
        print(('\t==> ' + text).encode('utf-8'))

        quad = barcode.quad
        cv.polylines(image, [np.array([(p.x, p.y) for p in quad], dtype=np.int32)],
                     isClosed=True, color=(0, 255, 0), thickness=2)


def barcode_live(input_device: str, use_display: bool = True, use_tensorrt: bool = False):
    license_info = get_license_info()
    if license_info.status != LicenseStatus.OKAY:
        print("No valid license")
        return

    configuration = BarcodeScannerConfiguration(
        processing_mode=ProcessingMode.AUTO,
        barcode_format_configurations=[
            BarcodeFormatCommonConfiguration(formats=BarcodeFormats.common)
        ]
    )
    if use_tensorrt:
        configuration.accelerator = TensorRtAccelerator(engine_path=os.curdir)

    scanner = BarcodeScanner(configuration=configuration)

    DISPLAY_HEIGHT = 640

    try:
        cap = cv.VideoCapture(int(input_device), cv.CAP_V4L2)
        cap.set(cv.CAP_PROP_FRAME_WIDTH, DEVICE_CAPTURE_WIDTH)
        cap.set(cv.CAP_PROP_FRAME_HEIGHT, DEVICE_CAPTURE_HEIGHT)
        cap.set(cv.CAP_PROP_FPS, DEVICE_CAPTURE_FRAMERATE)
    except ValueError:
        cap = cv.VideoCapture(gstreamer_pipeline(input_device), cv.CAP_GSTREAMER)

    if not cap.isOpened():
        print("Cannot open camera")
        return

    try:
        if use_display:
            cv.namedWindow("BarcodeRecognizer", cv.WINDOW_AUTOSIZE)

        image = np.zeros((DEVICE_CAPTURE_HEIGHT, DEVICE_CAPTURE_WIDTH, 3), dtype=np.uint8)
        while True:
            ret, _ = cap.read(image)
            if not ret:
                print("Failed to capture image")
                break

            with ImageRef.from_ndarray(image, RawImageLoadOptions(live_source=True)) as image_ref:
                result = scanner.run(image=image_ref)
                _parse_barcodes(result.barcodes, image)

                if use_display:
                    width = image.shape[1] * DISPLAY_HEIGHT / image.shape[0]
                    to_show = cv.resize(image, (int(width), DISPLAY_HEIGHT))
                    cv.imshow("BarcodeRecognizer", to_show)
                    cv.waitKey(1)

                    if cv.getWindowProperty("BarcodeRecognizer", cv.WND_PROP_AUTOSIZE) == -1:
                        break
    finally:
        cap.release()
