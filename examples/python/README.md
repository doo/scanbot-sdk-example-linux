# API Documentation
The Python API documentation is available [here](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/scanbotsdk_8py.html). 

## Raspberry Pi and Ubuntu

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
pip install https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/HZMdMdpCxVuaro3GE4G2gMmt/scanbotsdk-0.0.1-py3-none-linux_aarch64.whl
```

* Only on x64: Install the x86_64 Scanbot SDK package:

```bash
pip install https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/HZMdMdpCxVuaro3GE4G2gMmt/scanbotsdk-0.0.1-py3-none-linux_x86_64.whl
```

* Test that the scanbotsdk package has been installed correctly:

```bash
python -c "import scanbotsdk"
```

* You're now ready to run the examples.

---

## NVidia Jetson

> Scanbot SDK requires Jetpack 4.6.1, CUDA 10.2 and TensorRT 8.2.1 to run with GPU acceleration.

* Install Python and OpenCV

```bash
sudo apt install -y python3-venv nvidia-opencv
```

* Optionally, install CUDA and TensorRT for GPU acceleration. Make sure that you're running a supported Jetpack version.

```bash
sudo apt install -y nvidia-l4t-cuda libnvinfer8 libnvinfer-plugin8 libnvonnxparsers8
```

* Create a virtual environment:

```bash
python3 -m venv .env --system-site-packages
source .env/bin/activate
pip install --upgrade pip setuptools wheel
```

* Install the Scanbot SDK package

```bash
pip install https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/HZMdMdpCxVuaro3GE4G2gMmt/scanbotsdk-0.0.1-py2.py3-none-linux_aarch64.whl
```

* Test that the scanbotsdk package has been installed correctly:

```bash
python -c "import scanbotsdk"
```

* You're now ready to run the examples.
