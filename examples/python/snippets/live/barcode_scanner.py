import cv2 as cv
import numpy as np

from scanbotsdk import (
    BarcodeScanner,
    BarcodeScannerConfiguration,
    BarcodeScannerResult,
    ProcessingMode,
    BarcodeFormatCommonConfiguration,
    BarcodeFormats,
    TensorRtAccelerator
)


def create_barcode_scanner(use_tensorrt: bool = False) -> BarcodeScanner:
    cfg = BarcodeScannerConfiguration(
        processing_mode=ProcessingMode.AUTO,
        barcode_format_configurations=[
            BarcodeFormatCommonConfiguration(formats=BarcodeFormats.common)
        ],
    )
    if use_tensorrt:
        cfg.accelerator = TensorRtAccelerator(engine_path=".")
    return BarcodeScanner(cfg)


def print_barcodes_text(res: BarcodeScannerResult, frame):
    for bc in res.barcodes:
        print(bc.text.strip())
    return None

def draw_barcodes_frame(res: BarcodeScannerResult, frame, color=(0, 255, 0), thickness=2):
    for bc in res.barcodes:
        q = bc.quad
        pts = np.array(
            [[q[0].x, q[0].y], [q[1].x, q[1].y], [q[2].x, q[2].y], [q[3].x, q[3].y]],
            dtype=np.int32,
        )
        cv.polylines(frame, [pts], True, color, thickness, cv.LINE_AA)
        txt = bc.text.replace(" ", "").replace("\n", "").replace("\t", "")
        cv.putText(
            frame,
            txt,
            (pts[0][0], max(0, pts[0][1] - 6)),
            cv.FONT_HERSHEY_SIMPLEX,
            0.5,
            color,
            1,
            cv.LINE_AA,
        )
    return frame
