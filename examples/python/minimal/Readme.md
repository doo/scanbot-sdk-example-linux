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

The example simply expects an image file as input and prints the detected barcodes on the terminal. To run it with 
the provided example image, simply run from the above directory:

    python detect_barcodes.py  ../../example.png