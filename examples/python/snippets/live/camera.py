import sys

import cv2 as cv


WIDTH, HEIGHT, FPS = 1280, 720, 30


def _gst_pipeline(device: str) -> str:
    if device == "jetson_csi":
        return (
            "nvarguscamerasrc ! "
            f"video/x-raw(memory:NVMM), width=(int){WIDTH}, height=(int){HEIGHT}, "
            f"framerate=(fraction){FPS}/1 ! "
            "nvvidconv flip-method=0 ! "
            "video/x-raw, format=(string)BGRx ! "
            "videoconvert ! "
            "video/x-raw, format=(string)BGR ! "
            "appsink drop=1"
        )

    if device == "libcamera":
        return (
            "libcamerasrc ! "
            "video/x-raw, format=BGR, "
            f"width=(int){WIDTH}, height=(int){HEIGHT}, "
            f"framerate=(fraction){FPS}/1 ! "
            "appsink drop=1"
        )

    raise ValueError(f"Invalid GStreamer device: {device!r}")


def _backends_for_index() -> tuple[int, ...]:
    if sys.platform.startswith("win"):
        return (cv.CAP_MSMF, cv.CAP_DSHOW)

    if sys.platform == "darwin":
        return (cv.CAP_AVFOUNDATION,)

    if sys.platform.startswith("linux"):
        return (cv.CAP_V4L2,)

    return (cv.CAP_ANY,)


def _configure_camera(cap: cv.VideoCapture) -> None:
    properties = (
        (cv.CAP_PROP_FRAME_WIDTH, WIDTH),
        (cv.CAP_PROP_FRAME_HEIGHT, HEIGHT),
        (cv.CAP_PROP_FPS, FPS),
    )

    for prop, value in properties:
        cap.set(prop, value)


def _open_index(index: int) -> cv.VideoCapture:
    for backend in _backends_for_index():
        cap = cv.VideoCapture(index, backend)

        if not cap.isOpened():
            cap.release()
            continue

        _configure_camera(cap)
        return cap

    raise RuntimeError(f"Cannot open camera index {index}")


def open_camera(device: str) -> cv.VideoCapture:
    """Open a numeric camera index or a supported GStreamer pipeline."""

    try:
        index = int(device)
    except ValueError:
        pipeline = _gst_pipeline(device)
        cap = cv.VideoCapture(pipeline, cv.CAP_GSTREAMER)

        if not cap.isOpened():
            cap.release()
            raise RuntimeError(
                f"Cannot open GStreamer pipeline for {device!r}"
            )

        return cap

    return _open_index(index)