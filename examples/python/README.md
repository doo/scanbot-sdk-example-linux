# API Documentation

The Python API documentation is
available [here](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/python-api/).

## NVidia Jetson

> Scanbot SDK requires Jetpack 6.1, CUDA 12.6 and TensorRT 10.3 to run with GPU acceleration.

* Install Python and OpenCV

```bash
sudo apt install -y python3-venv nvidia-opencv
```

* Optionally, install CUDA and TensorRT for GPU acceleration. Make sure that you're running a supported Jetpack version.

```bash
sudo apt install -y nvidia-l4t-cuda libnvinfer10 libnvinfer-plugin10 libnvonnxparsers10
```

* Create a virtual environment:

```bash
python3 -m venv .env --system-site-packages
source .env/bin/activate
pip install --upgrade pip setuptools wheel
```

* Install the Scanbot SDK package

```bash
# specify the SDK-VERSION and SDK-API-TOKEN as specified along with the license file
export SCANBOTSDK_VERSION=<SDK-VERSION>
export SCANBOTSDK_API_TOKEN=<SDK-API-TOKEN>
pip install https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/${SCANBOTSDK_API_TOKEN}/scanbotsdk-${SCANBOTSDK_VERSION}-py3-none-linux_aarch64.whl
```

* Test that the scanbotsdk package has been installed correctly:

```bash
python -c "import scanbotsdk"
```

* You're now ready to run the examples.

### Important performance notes

* In order to achieve the best performance,
  it is important to set GPU, CPU and memory frequencies to the maximum and accelerate the fan to prevent overheating.
  This can be done using the `jetson_clocks` script:

```bash
sudo jetson_clocks --store
sudo jetson_clocks
```

You can then restore the default settings using:

```bash
sudo jetson_clocks --restore
```

If setting the frequencies to the maximum is not desired, you can modify the `jetson_clocks` script to set the
frequencies to a desired value.

---

## Raspberry Pi OS, Ubuntu, Debian

* Install Python and OpenCV:

```bash
sudo apt install -y python3-venv python3-opencv
```

* Create a virtual environment:

```bash
python3 -m venv .env --system-site-packages
source .env/bin/activate
pip install --upgrade pip setuptools wheel
```

* Only on ARM64: Install the ARM64 Scanbot SDK package:

```bash
# specify the SDK-VERSION and SDK-API-TOKEN as specified along with the license file
export SCANBOTSDK_VERSION=<SDK-VERSION>
export SCANBOTSDK_API_TOKEN=<SDK-API-TOKEN>
pip install https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/${SCANBOTSDK_API_TOKEN}/scanbotsdk-${SCANBOTSDK_VERSION}-py3-none-linux_aarch64.whl
```

* Only on x64: Install the x86_64 Scanbot SDK package:

```bash
# specify the SDK-VERSION and SDK-API-TOKEN as specified along with the license file
export SCANBOTSDK_VERSION=<SDK-VERSION>
export SCANBOTSDK_API_TOKEN=<SDK-API-TOKEN>
pip install https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/${SCANBOTSDK_API_TOKEN}/scanbotsdk-${SCANBOTSDK_VERSION}-py3-none-linux_x86_64.whl
```

* Test that the scanbotsdk package has been installed correctly:

```bash
python -c "import scanbotsdk"
```

* You're now ready to run the examples.
