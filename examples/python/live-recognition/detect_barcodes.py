from scanbotsdk import *
import argparse
import cv2 as cv
import os 
import time

LICENSE_KEY:str = "Put-your-license-key-here"

def gstreamer_pipeline(device):
    DEVICE_CAPTURE_WIDTH = 1280
    DEVICE_CAPTURE_HEIGHT = 720
    DEVICE_CAPTURE_FRAMERATE = 30

    if device == "mipi":
        ret = "nvarguscamerasrc ! "
        ret += f"video/x-raw(memory:NVMM), width=(int){DEVICE_CAPTURE_WIDTH}, height=(int){DEVICE_CAPTURE_HEIGHT}, "
        ret += f"framerate=(fraction){DEVICE_CAPTURE_FRAMERATE}/1 ! nvvidconv flip-method=0 ! "
        ret += f"video/x-raw, format=(string)BGRx ! "
        ret += "videoconvert ! video/x-raw, format=(string)BGR ! appsink drop=1"
    elif device == "usb":
        ret = "v4l2src device=/dev/video1 ! "
        ret += f"image/jpeg,format=MJPG,width=(int){DEVICE_CAPTURE_WIDTH},height=(int){DEVICE_CAPTURE_HEIGHT},"
        ret += f"framerate=(fraction){DEVICE_CAPTURE_FRAMERATE}/1 ! "
        ret += f"nvv4l2decoder mjpeg=1 ! nvvidconv ! video/x-raw,format=BGRx ! "
        ret += "videoconvert ! video/x-raw,format=BGR ! appsink drop=1"
    else:
        raise ValueError("Invalid device name: " + device)

    return ret

def parseBarcodes(barcodes, image):
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
        
def recognizeLive(recognizer:BarcodeRecognizer, device:str, use_display:bool):

    DISPLAY_HEIGHT = 640
    cap = cv.VideoCapture(gstreamer_pipeline(device), cv.CAP_GSTREAMER)
    if not cap.isOpened():
        print("Cannot open camera")
        exit(0)
    
    if use_display:
        cv.namedWindow("BarcodeRecognizer", cv.WINDOW_AUTOSIZE)

    while(True):
        _, image = cap.read()
        if image is None:
            print("Failed to capture image")
            break

        barcodes = recognizer.recognize(image)
        parseBarcodes(barcodes, image)

        if use_display:
            width = image.shape[1] * DISPLAY_HEIGHT / image.shape[0]
            image = cv.resize(image, (int(width), DISPLAY_HEIGHT))
            cv.imshow("BarcodeRecognizer", image)
            cv.waitKey(1)

            if cv.getWindowProperty("BarcodeRecognizer", cv.WND_PROP_AUTOSIZE) == -1:
                break
        else:
            cv.imwrite("result.jpg", image)
        
    cap.release()
    return


def recognize():
    parser = argparse.ArgumentParser(description='Detect barcodes in an image.')
    parser.add_argument('--input', type=str, required=True, help='Input device name (usb|mipi) for live recognition')
    # parse an optional flag use_display
    parser.add_argument('--use_display', action='store_true', help='Use display for live recognition')
    

    args = parser.parse_args()
    input_device = args.input
    use_display = args.use_display
    
    # Setup and initialize the Scanbot SDK
    print(f"Initializing Scanbot SDK...")
    sdk = ScanbotSDK(LICENSE_KEY)
    print(f"License Staus: {sdk.get_license_status()}")

    recognizer = BarcodeRecognizer(sdk, BarcodeRecognitionEngineMode.FAST)
    recognizeLive(recognizer, input_device, use_display)    



if __name__ == '__main__':
    recognize()