# Scanbot SDK for Linux – C Example

### NVidia Jetson

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

### Raspberry Pi OS, Ubuntu, Debian

* Install a C compiler, CMake and wget:

```bash
sudo apt install -y cmake build-essential wget
```

* You're now ready to build the examples.

## Building the Examples

In order to build all examples, run the following commands:

```bash
mkdir build
cd build
# Replace `<SCANBOTSDK_VERSION>` with the actual version number of the SDK you want to install.
cmake -DSCANBOTSDK_VERSION=<SCANBOTSDK_VERSION> ..
make
```

## Usage
The example supports four modes: **scan**, **analyze**, **classify**, and **parse**.
```bash
./scanbotsdk_example scan <subcommand> --file <path/to/file.jpg> [--license <KEY>]
./scanbotsdk_example scan <subcommand> --file <path/to/file.jpg> [--license <KEY>]
./scanbotsdk_example analyze <subcommand> --file <path/to/file.jpg> [--save <out.jpg>] [--license <KEY>]
./scanbotsdk_example analyze <subcommand> --file <path/to/file.jpg> [--save <out.jpg>] [--license <KEY>]
./scanbotsdk_example classify <subcommand> --file <path/to/file.jpg> [--license <KEY>]
./scanbotsdk_example parse <subcommand> --text "<input>" [--license <KEY>]
```

## Example
```bash
./scanbotsdk_example scan barcode --file images/example.jpg --license <KEY>
./scanbotsdk_example analyze analyze_multi_page --file files/doc.pdf --license <KEY>
./scanbotsdk_example analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>
./scanbotsdk_example parse mrz --text "P<UTOERIKSSON<<ANNA<MARIA<<<<<<" --license <KEY>
```

## API References
👉 [Scanbot SDK - C API](https://api-docs.scanbot.io/document-scanner-sdk/linux/c-api/index.html)
