# Scanbot SDK for Linux – C Example

### NVidia Jetson

> Scanbot SDK requires Jetpack 6.1, CUDA 12.6 and TensorRT 10.3 to run with GPU acceleration.

* Install a C compiler and CMake:

```bash
sudo apt install -y cmake build-essential
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

### Raspberry Pi OS, Ubuntu, Debian

* Install a C compiler and CMake:

```bash
sudo apt install -y cmake build-essential
```

* You're now ready to build the examples.

### Windows

Requirements:

* Visual Studio C++ Build Tools
* CMake 3.10+
* Recommended: ninja

## Building the Examples

In order to build all examples, run the following commands:

* With ninja (recommended):
    ```bash
    mkdir build
    cd build
    # Replace `<SCANBOTSDK_VERSION>` with the actual version number of the SDK you want to install.
    cmake -GNinja -DSCANBOTSDK_VERSION=<SCANBOTSDK_VERSION> ..
    ninja
    ```

* With make:

    ```bash
    mkdir build
    cd build
    # Replace `<SCANBOTSDK_VERSION>` with the actual version number of the SDK you want to install.
    cmake -DSCANBOTSDK_VERSION=<SCANBOTSDK_VERSION> ..
    make
    ```

* With MS Visual Studio (Windows-only):

    ```powershell
    mkdir build
    cd build
    # Replace `<SCANBOTSDK_VERSION>` with the actual version number of the SDK you want to install.
    cmake "-DSCANBOTSDK_VERSION=<SCANBOTSDK_VERSION>" ..
    msbuild scanbotsdk_c_example.sln
    # scanbotsdk_example.exe will be created under Debug\
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
./scanbotsdk_example live <subcommand> --file <path/to/file.jpg> [--license <KEY>] [--use_tensorrt]

```

## Example
```bash
./scanbotsdk_example scan barcode --file images/example.jpg --license <KEY>
./scanbotsdk_example analyze analyze_multi_page --file files/doc.pdf --license <KEY>
./scanbotsdk_example analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>
./scanbotsdk_example parse mrz --text "P<UTOERIKSSON<<ANNA<MARIA<<<<<<" --license <KEY>
./scanbotsdk_example live barcode --file images/example.jpg --license <KEY>
```

## API References
👉 [Scanbot SDK - C API](https://api-docs.scanbot.io/document-scanner-sdk/linux/c-api/index.html)
