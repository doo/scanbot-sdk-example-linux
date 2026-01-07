import cv2 as cv

# Default camera settings (resolution and framerate)
WIDTH, HEIGHT, FPS = 1280, 720, 30

def _gst_pipeline(device: str) -> str:
    if device == "jetson_csi":
        return (
            "nvarguscamerasrc ! "
            f"video/x-raw(memory:NVMM), width=(int){WIDTH}, height=(int){HEIGHT}, "
            f"framerate=(fraction){FPS}/1 ! "
            "nvvidconv flip-method=0 ! "
            "video/x-raw, format=(string)BGRx ! "
            "videoconvert ! video/x-raw, format=(string)BGR ! appsink drop=1"
        )
    if device == "libcamera":
        return (
            "libcamerasrc ! video/x-raw, format=BGR, "
            f"width=(int){WIDTH}, height=(int){HEIGHT}, "
            f"framerate=(fraction){FPS}/1 ! appsink"
        )
    raise ValueError(f"Invalid device for GStreamer: {device!r}")


def open_camera(device: str) -> cv.VideoCapture:
    """Open V4L2 (numeric index) or GStreamer ('jetson_csi' / 'libcamera')."""
    try:
        cap = cv.VideoCapture(int(device), cv.CAP_V4L2)
        cap.set(cv.CAP_PROP_FRAME_WIDTH, WIDTH)
        cap.set(cv.CAP_PROP_FRAME_HEIGHT, HEIGHT)
        cap.set(cv.CAP_PROP_FPS, FPS)
    except ValueError:
        cap = cv.VideoCapture(_gst_pipeline(device), cv.CAP_GSTREAMER)
    if not cap.isOpened():
        raise RuntimeError("Cannot open camera")
    return cap
