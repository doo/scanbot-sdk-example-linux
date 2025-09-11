# API Documentation
The API documentation is accessible here:
* [Scanbot SDK - C API](https://api-docs.scanbot.io/document-scanner-sdk/linux/c-api/index.html)

# Installation

## NVidia Jetson

> Scanbot SDK requires Jetpack 6.1, CUDA 12.6 and TensorRT 10.3 to run with GPU acceleration.

* Install a C compiler, CMake and wget:

```bash
sudo apt install -y cmake build-essential wget
```

* Optionally, install CUDA and TensorRT for GPU acceleration. Make sure that you're running a supported Jetpack version.

```bash
sudo apt install -y nvidia-l4t-cuda libnvinfer10 libnvinfer-plugin10 libnvonnxparsers10
```

### ⚡ Performance notes (Jetson)
To avoid throttling, set max GPU/CPU/memory clocks:
```bash
sudo jetson_clocks --store
sudo jetson_clocks
```
Restore defaults later:
```bash
sudo jetson_clocks --restore
```

* You're now ready to run the examples.

----

## Raspberry Pi OS, Ubuntu, Debian

* Install a C compiler, CMake and wget:

```bash
sudo apt install -y cmake build-essential wget
```

* You're now ready to build the examples.

# Building the Examples
In order to build all examples, run the following commands:

```bash
mkdir build
cd build
cmake -DSCANBOTSDK_VERSION=<SCANBOTSDK_VERSION> ..
make
```

Replace `<SCANBOTSDK_VERSION>` with the actual version number of the SDK you want to install.

# Running the Examples
See the `README.md` files in the individual example directories for instructions on how to run them.