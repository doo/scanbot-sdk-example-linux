This example demonstrates how to recognize barcodes in a live video feed.

----

## Pre-requisites

Follow the instructions in [README.md](../README.md) to set up your environment.

You will also need a USB webcam or a CSI camera to provide the live video feed.

----
## License Key

Please update the license key in `src/main.cpp` before running the example.

## Installation

Go to the `examples/c++/live-recognition` directory in a terminal and run the following commands:

```bash
mkdir build
cd build
cmake -DSCANBOTSDK_VERSION=<SDK-VERSION> -DSCANBOTSDK_API_TOKEN=<SDK-API-TOKEN> ..
make
```

_The SDK-VERSION and the SDK-API-TOKEN should have been specified alongwith the license file._

----
## Usage

The example will connect to the selected camera and begin looking for barcodes. It will print any detected barcodes to the terminal.

The example can be run as follows from the terminal:

* Using a CSI camera on the Jetson Nano:

```bash
./detect_barcodes -i jetson_csi
```

* Using a CSI camera with libcamera on PiOS:

```bash
./detect_barcodes -i libcamera
```

* Using a camera compatible with video4linux, e.g. a USB webcam (available as /dev/video1 in this example; change the device number as per your system):

```bash
./detect_barcodes -i 1
```

### Optional flags

* ```--use-display```: Display a window that shows the found barcodes live. The found barcodes will be indicated with a green border overlaid on top.

* ```--use-tensorrt```: Enable TensorRT acceleration (Jetson only).
