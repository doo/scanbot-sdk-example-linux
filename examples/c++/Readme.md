
This example demonstrates how to embed the ScanbotSDK for barcode recognition in a C++ environment.

----

## Pre-requisites

* ScanbotSDK for Linux 
   * libscanbotsdk
   * valid license string for barcode recognition

* Other than an valid C++ compiler, following tools are expected on the host for running this example
   * [cmake](https://cmake.org/)
   * [opencv](https://opencv.org/)
   * [gstreamer](https://gstreamer.freedesktop.org/) : for the live camera feed

* Webcam or mipi camera device for capturing live stream for barcode recognition

----
## Installation

Please navigate to the examples/c++ directory and run the following commands from the terminal:

    cd <path-to-examples-c++>
    mkdir libs
    cp -r <path-to-libscanbotsdk> libs/
    mkdir build
    cd build
    cmake ..
    make

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

    <path-to-examples-c++>/build/detect_barcodes -i mipi -l <path-to-license-file>

Using usb-webcam device:

    <path-to-examples-c++>/build/detect_barcodes -i usb -l <path-to-license-file>


_Note that the webcam is expected to be /dev/video1 in the system. In case the camera is mounted on another device, please update the gstreamer_pipeline string accordingly in main.cpp._

----
### Recognition in Static mode

In static mode, an image file is expected to be provided as input. The example will then display the image with the highlighted ROI and print the barcode texts on the terminal.

    <path-to-examples-c++>/build/detect_barcodes -i <path-to-image-file> -l <path-to-license-file>