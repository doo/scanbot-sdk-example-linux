import cv2 as cv
from scanbotsdk import ImageRef, RawImageLoadOptions

from snippets.live.preview import preview_on_frame

def run_scanner(cap: cv.VideoCapture, scan, on_result):
    """Read frames, run scanner, and show processed frame if returned."""
    show_preview = None
    try:
        while True:
            ok, frame = cap.read()
            if not ok:
                break

            with ImageRef.from_ndarray(frame, RawImageLoadOptions(live_source=True)) as img:
                res = scan(image=img)

            frame_to_show = on_result(res, frame)
            if frame_to_show is not None:
                if show_preview is None:
                    show_preview = preview_on_frame("Live")
                show_preview(frame_to_show)
    except KeyboardInterrupt:
        pass
    finally:
        cap.release()
