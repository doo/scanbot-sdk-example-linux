The python package `scanbotsdk` is a wrapper around the C library `libscanbotsdk.so` and provides an easy pythonic interface to the ScanbotSDK for barcode recognition.

This example demonstrates how to embed and use it for a simple barcode recognition task.

----

## Pre-requisites

* ScanbotSDK license for Linux 
   * valid license string

* python 3.6 or higher
* python3-pip
* Expected Python packages:
   * opencv-python

----
## License Key

Please update the license key in detect_barcodes.py before running the example.

## Installation

Install the scanbotsdk package:

    pip install  https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/HZMdMdpCxVuaro3GE4G2gMmt/scanbotsdk-0.0.1-py3-none-any.whl

To test that scanbotsdk is installed correctly, run the following command:

    python -c "import scanbotsdk"

If the installation was successful, the command should not return any error.

----
## Usage

In live mode, a camera device is expected to be connected to the host. By default, the example will print the detected
barcodes on the terminal.
If "--use-display" flag is enable, then it will open a window and display the frames on each recognition step and an 
ROI will be drawn around the detected barcodes in the live feed. 

The example can be run as follows from the terminal:

Using mipi camera device:

    python ./detect_barcodes --input mipi [--use-display]

Using usb-webcam device:

    python ./detect_barcodes --input usb [--use-display]


_Note that the webcam is expected to be /dev/video1 in the system. In case the camera is mounted on another device, please update the gstreamer_pipeline string accordingly in main.cpp._