The python package `scanbotsdk` is a wrapper around the C library `libscanbotsdk.so` and provides an easy pythonic interface to the ScanbotSDK for barcode recognition.

This example demonstrates how to embed and use it for a simple barcode recognition task.

----

## Pre-requisites

* ScanbotSDK for Linux 
   * libscanbotsdk
   * scanbotsdk-pypkg (provided as part of this example)
   * valid license string for barcode recognition

* python 3.6 or higher
* python3-pip
* Expected Python packages:
   * opencv-python
   * click

----

## Installation

Install the scanbotsdk package:

    cd <path-to-examples-python>
    pip install scanbotsdk-pypkg/scanbotsdk-0.0.1-py2.py3-none-any.whl

To test that scanbotsdk is installed correctly, run the following command:

    python -c "import scanbotsdk"

If the installation was successful, the command should not return any error.

----
## Usage

The example can be run in two modes:

1. Live mode : using a camera device
2. Static mode: using a static image file

----
### Recognition in Live mode

In live mode, a camera device is expected to be connected to the host. The example will then open a window and display the frames on each recognition step. The detected barcodes at each step wiil be printed on the terminal and an ROI will be drawn around the detected barcodes in the live feed.

The example can be run as follows from the terminal:

Using mipi camera device:

    python detect_barcodes.py --sdk <path-to-libscanbotsdk.so> --license <path-to-license-file> -input mipi

Using usb-webcam device:
    
    python detect_barcodes.py --sdk <path-to-libscanbotsdk.so> --license <path-to-license-file> -input usb

_Note that the webcam is expected to be /dev/video1 in the system. In case the camera is mounted on another device, please update the gstreamer_pipeline string accordingly in detect_barcodes.py._


----
### Recognition in Static mode

In static mode, an image file is expected to be provided as input. The example will then display the image with the highlighted ROI and print the barcode texts on the terminal.

The example can be run as follows from the terminal:

    python detect_barcodes.py --sdk <path-to-libscanbotsdk.so> --license <path-to-license-file> -input <path-to-image-file> 