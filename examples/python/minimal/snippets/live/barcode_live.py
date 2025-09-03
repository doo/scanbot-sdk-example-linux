import os
import numpy as np
import cv2 as cv
from scanbotsdk import *
from snippets.live.camera import Camera

BARCODE_POLY_COLOR = (0, 255, 0)
BARCODE_POLY_THICKNESS = 2

def barcode_live(input_device: str, use_display: bool = True, use_tensorrt: bool = False) -> None:
    configuration = BarcodeScannerConfiguration(
        processing_mode=ProcessingMode.AUTO,
        barcode_format_configurations=[
            BarcodeFormatCommonConfiguration(formats=BarcodeFormats.common)
        ],
    )
    if use_tensorrt:
        configuration.accelerator = TensorRtAccelerator(engine_path=os.curdir)

    scanner = BarcodeScanner(configuration=configuration)

    def on_frame(frame: np.ndarray):
        # Although it is not required to use the image ref as a context manager, it is recommended to do so when
        # creating from live source to ensure that the image_ref is timely released.
        #
        # Setting live_source=True lets the scanner know that we're running in live mode.
        # In this mode we maintain the highest FPS because we spread the work of scanning barcodes across multiple frames.
        # If you set live_source=False, the scanner will run in single-shot mode which is much slower,
        # but has a much higher probability of finding all barcodes in the input image.
        # As an alternative, you can explicitly set the scanner mode,
        # by creating it with e.g. processing_mode=ProcessingMode.LIVE for live mode.
        with ImageRef.from_ndarray(frame, RawImageLoadOptions(live_source=True)) as image_ref:
            result = scanner.run(image=image_ref)
            for barcode in result.barcodes:
                # remove all white-spaces, newlines and tabs from the barcode text
                cleaned_text = barcode.text.replace(" ", "").replace("\n", "").replace("\t", "")
                print(('\t==> ' + cleaned_text).encode('utf-8'))
                
                # draw the ROI with individual line
                quad = barcode.quad
                cv.line(frame, (quad[0].x, quad[0].y), (quad[1].x, quad[1].y), BARCODE_POLY_COLOR, BARCODE_POLY_THICKNESS)
                cv.line(frame, (quad[1].x, quad[1].y), (quad[2].x, quad[2].y), BARCODE_POLY_COLOR, BARCODE_POLY_THICKNESS)
                cv.line(frame, (quad[2].x, quad[2].y), (quad[3].x, quad[3].y), BARCODE_POLY_COLOR, BARCODE_POLY_THICKNESS)
                cv.line(frame, (quad[3].x, quad[3].y), (quad[0].x, quad[0].y), BARCODE_POLY_COLOR, BARCODE_POLY_THICKNESS)

    Camera(input_device, use_display).run(on_frame)
