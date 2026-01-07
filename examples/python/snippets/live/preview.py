import cv2 as cv

def preview_on_frame(window_name: str = "Live"):
    cv.namedWindow(window_name, cv.WINDOW_AUTOSIZE)

    def _on_frame(frame):
        cv.imshow(window_name, frame)
        if cv.waitKey(1) == 27:  # ESC
            cv.destroyAllWindows()
            raise KeyboardInterrupt

    return _on_frame