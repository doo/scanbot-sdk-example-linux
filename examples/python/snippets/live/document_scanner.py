import cv2 as cv
import numpy as np
from scanbotsdk import DocumentDetectionResult, DocumentScanner, DocumentScannerConfiguration, ProcessingMode


def create_document_scanner() -> DocumentScanner:
    config = DocumentScannerConfiguration(
        processing_mode=ProcessingMode.AUTO,
    )
    return DocumentScanner(configuration=config)


def print_documents_text(res: DocumentDetectionResult, frame):
    print(res.status.name)
    return None


def draw_documents_frame(res: DocumentDetectionResult, frame, color=(0, 255, 0), thickness=2):
    pts_list = list(res.points or [])
    if not pts_list and res.points_normalized:
        h, w = frame.shape[:2]
        pts = np.array([[int(p.x * w), int(p.y * h)] for p in res.points_normalized], dtype=np.int32)
    elif pts_list:
        pts = np.array([[int(p.x), int(p.y)] for p in pts_list], dtype=np.int32)
    else:
        return frame

    # Draw contour
    cv.polylines(frame, [pts], True, color, thickness, cv.LINE_AA)

    # Put status label above the top-most vertex
    top_idx = int(np.argmin(pts[:, 1]))
    x, y = int(pts[top_idx, 0]), int(pts[top_idx, 1])
    y = max(0, y - 6)
    label = res.status.name
    cv.putText(frame, label, (x, y), cv.FONT_HERSHEY_SIMPLEX, 0.5, color, 1, cv.LINE_AA)

    return frame