import cv2 as cv
import numpy as np
from scanbotsdk import DocumentDetectionResult, DocumentScanner, DocumentScannerConfiguration, ProcessingMode


def create_document_scanner() -> DocumentScanner:
    cfg = DocumentScannerConfiguration(
        processing_mode=ProcessingMode.AUTO,
    )
    return DocumentScanner(cfg)


def print_documents_text(res: DocumentDetectionResult, frame):
    print(res.status.name)
    return None


def draw_documents_frame(res: DocumentDetectionResult, frame, color=(0, 255, 0), thickness=2):
    pts = np.array([[p.x, p.y] for p in res.points], dtype=np.int32)
    cv.polylines(frame, [pts], True, color, thickness, cv.LINE_AA)
    label = res.status.name
    anchor = pts[0]
    cv.putText(
        frame,
        label,
        (int(anchor[0]), max(0, int(anchor[1]) - 6)),
        cv.FONT_HERSHEY_SIMPLEX,
        0.5,
        color,
        1,
        cv.LINE_AA,
    )
    return frame