# API Documentation
The API documentation is accessible here: 
* [Scanbot SDK - C API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/ScanbotSDK_8h.html)
* [Scanbot SDK - C++ API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/ScanbotSDK_8hpp.html)

# Installation

## NVidia Jetson

> Scanbot SDK requires Jetpack 4.6.1, CUDA 10.2 and TensorRT 8.2.1 to run with GPU acceleration.

* Install a C++ compiler, CMake and OpenCV:

```bash
sudo apt install -y cmake build-essential nvidia-opencv
```

* Optionally, install CUDA and TensorRT for GPU acceleration. Make sure that you're running a supported Jetpack version.

```bash
sudo apt install -y nvidia-l4t-cuda libnvinfer8 libnvinfer-plugin8 libnvonnxparsers8
```

* You're now ready to run the examples.

----

## Raspberry Pi OS, Ubuntu, Debian

* Install a C++ compiler, CMake and OpenCV:

```bash
sudo apt install -y cmake build-essential libopencv-dev
```

* You're now ready to run the examples.
