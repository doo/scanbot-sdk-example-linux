
This example demonstrates how to embed the ScanbotSDK for live barcode-recognition, from an input device, in a C++ environment.

----

## Pre-requisites

* ScanbotSDK license for Linux 
   * valid license string for barcode recognition

* Other than an valid C++ compiler, following tools are expected on the host for running this example
   * [cmake](https://cmake.org/)
   * [opencv](https://opencv.org/)
   * [gstreamer](https://gstreamer.freedesktop.org/) : for the live camera feed

* Webcam or mipi camera device for capturing live stream for barcode recognition

----
## License Key

Please update the license key in main.cpp before running the example.

## Installation

Please navigate to the examples/c++ directory and run the following commands from the terminal:


    cd <path-to-examples-c++>
    mkdir build
    cd build
    cmake ..
    make

_Note that the scanbotsdk library will be downloaded and placed at libs/scanbotsdk during the cmake configuration step._

----
## Usage

In live mode, a camera device is expected to be connected to the host. By default, the example will print the detected
barcodes on the terminal.
If "--use-display" flag is enable, then it will open a window and display the frames on each recognition step and an 
ROI will be drawn around the detected barcodes in the live feed. 

The example can be run as follows from the terminal:

Using mipi camera device:

    ./detect_barcodes -i mipi [--use-display]

Using usb-webcam device:

    ./detect_barcodes -i usb [--use-display]


_Note that the webcam is expected to be /dev/video1 in the system. In case the camera is mounted on another device, please update the gstreamer_pipeline string accordingly in main.cpp._