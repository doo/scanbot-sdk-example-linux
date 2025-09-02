import cv2 as cv
import numpy as np
from typing import Callable

DEVICE_CAPTURE_WIDTH = 1280
DEVICE_CAPTURE_HEIGHT = 720
DEVICE_CAPTURE_FRAMERATE = 30
DISPLAY_HEIGHT = 640

BACKEND_V4L2 = cv.CAP_V4L2
BACKEND_GSTREAMER = cv.CAP_GSTREAMER

WINDOW_NAME = "ScanbotLive"
WAIT_DELAY_MS = 1

def gstreamer_pipeline(device: str) -> str:
    if device == "jetson_csi":
        return (
            "nvarguscamerasrc ! "
            f"video/x-raw(memory:NVMM), width=(int){DEVICE_CAPTURE_WIDTH}, height=(int){DEVICE_CAPTURE_HEIGHT}, "
            f"framerate=(fraction){DEVICE_CAPTURE_FRAMERATE}/1 ! "
            "nvvidconv flip-method=0 ! "
            "video/x-raw, format=(string)BGRx ! "
            "videoconvert ! video/x-raw, format=(string)BGR ! appsink drop=1"
        )
    if device == "libcamera":
        return (
            "libcamerasrc ! video/x-raw, format=BGR, "
            f"width=(int){DEVICE_CAPTURE_WIDTH}, "
            f"height=(int){DEVICE_CAPTURE_HEIGHT}, "
            f"framerate=(fraction){DEVICE_CAPTURE_FRAMERATE}/1 ! "
            "appsink"
        )
    raise ValueError(f"Invalid device name: {device}")


class Camera:
    """Encapsulates camera + display logic for live examples."""

    def __init__(self, input_device: str, use_display: bool = True):
        try:
            self.cap = cv.VideoCapture(int(input_device), BACKEND_V4L2)
            self.cap.set(cv.CAP_PROP_FRAME_WIDTH, DEVICE_CAPTURE_WIDTH)
            self.cap.set(cv.CAP_PROP_FRAME_HEIGHT, DEVICE_CAPTURE_HEIGHT)
            self.cap.set(cv.CAP_PROP_FPS, DEVICE_CAPTURE_FRAMERATE)
        except ValueError:
            self.cap = cv.VideoCapture(gstreamer_pipeline(input_device), BACKEND_GSTREAMER)

        if not self.cap.isOpened():
            raise RuntimeError("Cannot open camera")

        self.use_display = use_display
        if self.use_display:
            cv.namedWindow(WINDOW_NAME, cv.WINDOW_AUTOSIZE)

    def run(self, frame_callback: Callable[[np.ndarray], None]):
        try:
            while True:
                success, frame = self.cap.read()
                if not success:
                    print("Failed to capture image")
                    break

                frame_callback(frame)

                if self.use_display:
                    aspect_width = int(frame.shape[1] * DISPLAY_HEIGHT / frame.shape[0])
                    display_frame = cv.resize(frame, (aspect_width, DISPLAY_HEIGHT))
                    cv.imshow(WINDOW_NAME, display_frame)
                    cv.waitKey(WAIT_DELAY_MS)

                    if cv.getWindowProperty(WINDOW_NAME, cv.WND_PROP_VISIBLE) < 1:
                        break
        finally:
            self.cap.release()
