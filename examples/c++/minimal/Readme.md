
The example demonstrates how to minimally embed the ScanbotSDK for barcode recognition in a C++ environment.

----

## Pre-requisites

* ScanbotSDK license for Linux 
   * valid license string for barcode recognition

* Other than an valid C++ compiler, following tools are expected on the host for running this example
   * [cmake](https://cmake.org/)
   * [opencv](https://opencv.org/)

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

The example simply expects an image file as input and prints the detected barcodes on the terminal. To run it with 
the provided example image, simply run from the above directory:

    ./detect_barcodes ../../example.png