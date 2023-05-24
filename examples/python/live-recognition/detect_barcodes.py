import scanbotsdk
import argparse
import cv2 as cv

LICENSE_KEY:str = "Put-your-license-key-here"


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

def parse_barcodes(barcodes, image):
    print('Recognized barcodes:')
    for barcode in barcodes:
        # remove all white-spaces, newlines and tabs from the barcode text
        barcode.text = barcode.text.replace(' ', '').replace('\n', '').replace('\t', '')
        print('\t==> ' + barcode.text)

        # draw the ROI with individual line
        cv.line(image, (barcode.points[0], barcode.points[1]), (barcode.points[2], barcode.points[3]), (0, 255, 0), 2)
        cv.line(image, (barcode.points[2], barcode.points[3]), (barcode.points[4], barcode.points[5]), (0, 255, 0), 2)
        cv.line(image, (barcode.points[4], barcode.points[5]), (barcode.points[6], barcode.points[7]), (0, 255, 0), 2)
        cv.line(image, (barcode.points[6], barcode.points[7]), (barcode.points[0], barcode.points[1]), (0, 255, 0), 2)


def recognize_live(recognizer:scanbotsdk.BarcodeRecognizer, device:str, use_display:bool):
    DISPLAY_HEIGHT = 640

    try:
        cap = cv.VideoCapture(int(device), cv.CAP_V4L2)
        cap.set(cv.CAP_PROP_FRAME_WIDTH, DEVICE_CAPTURE_WIDTH)
        cap.set(cv.CAP_PROP_FRAME_HEIGHT, DEVICE_CAPTURE_HEIGHT)
        cap.set(cv.CAP_PROP_FPS, DEVICE_CAPTURE_FRAMERATE)
    except ValueError:
        cap = cv.VideoCapture(gstreamer_pipeline(device), cv.CAP_GSTREAMER)

    if not cap.isOpened():
        print("Cannot open camera")
        exit(1)
    
    if use_display:
        cv.namedWindow("BarcodeRecognizer", cv.WINDOW_AUTOSIZE)

    while(True):
        _, image = cap.read()
        if image is None:
            print("Failed to capture image")
            break

        barcodes = recognizer.recognize(image)
        parse_barcodes(barcodes, image)

        if use_display:
            width = image.shape[1] * DISPLAY_HEIGHT / image.shape[0]
            image = cv.resize(image, (int(width), DISPLAY_HEIGHT))
            cv.imshow("BarcodeRecognizer", image)
            cv.waitKey(1)

            if cv.getWindowProperty("BarcodeRecognizer", cv.WND_PROP_AUTOSIZE) == -1:
                break
                    
    cap.release()


def recognize():
    parser = argparse.ArgumentParser(description='Detect barcodes in an image.')
    parser.add_argument('--input', type=str, required=True, help='Input device name (jetson_csi|libcamera) or number of /dev/video* device')
    parser.add_argument('--use-display', action='store_true', help='Show results in a window')
    parser.add_argument('--use-tensorrt', action='store_true', help='Use TensorRT backend for GPU acceleration')
    

    args = parser.parse_args()
    input_device = args.input
    use_display = args.use_display
    
    # Setup and initialize the Scanbot SDK
    print(f"Initializing Scanbot SDK...")
    scanbotsdk.initialize(LICENSE_KEY)
    print(f"License Status: {scanbotsdk.get_license_status()}")

    # Setup recognition parameters
    params = scanbotsdk.BarcodeRecognizerInitParams(
        engine_mode=scanbotsdk.BarcodeRecognitionEngineMode.FAST,
    )

    if args.use_tensorrt:
        params.use_tensorrt = True

    recognizer = scanbotsdk.BarcodeRecognizer(params)
    recognize_live(recognizer, input_device, use_display)



if __name__ == '__main__':
    recognize()