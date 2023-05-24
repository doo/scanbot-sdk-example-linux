This example demonstrates the absolute minimum necessary to recognize barcodes in an image using the Scanbot SDK.

----

## Pre-requisites

Follow the instructions in [README.md](../README.md) to set up your environment.

----
## License Key

Please update the license key in `src/main.cpp` before running the example.

## Installation

Go to the `examples/c++/minial` directory in a terminal and run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

_Note that the scanbotsdk library will be downloaded and placed at libs/scanbotsdk during the cmake configuration step._

----
## Usage

The example takes an image file as input and prints the detected barcodes on the terminal. To run it with the provided example image, execute the following (from the build directory created above):

```bash
./detect_barcodes ../../../example.png
```
