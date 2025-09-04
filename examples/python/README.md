# Scanbot SDK – Python on Linux
This repository demonstrates how to use the [Scanbot SDK for Linux](https://scanbot.io) with Python.  

## NVidia Jetson

> Requires **Jetpack 6.1**, **CUDA 12.6**, and **TensorRT 10.3** for GPU acceleration.

### 1. Install Python and OpenCV
```bash
sudo apt install -y python3-venv nvidia-opencv
```

### 2. (Optional) Install CUDA + TensorRT
Make sure you are running a supported Jetpack version:
```bash
sudo apt install -y nvidia-l4t-cuda libnvinfer10 libnvinfer-plugin10 libnvonnxparsers10
```

### 3. Create and activate a virtual environment
```bash
python3 -m venv .env --system-site-packages
source .env/bin/activate
pip install --upgrade pip setuptools wheel
```

### 4. Download and install the Scanbot SDK
1. Go to [GitHub Releases](https://github.com/doo/scanbot-sdk-example-linux/releases)  
2. Manually download the **Python package** for your platform (ARM64 or x86_64).  
3. Install it with pip, for example:

```bash
pip install ./scanbotsdk-<VERSION>-py3-none-linux_aarch64.whl
```

or

```bash
pip install ./scanbotsdk-<VERSION>-py3-none-linux_x86_64.whl
```

### 5. Verify installation
```bash
python -c "import scanbotsdk"
```

✅ You're ready to run the examples!

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

## Raspberry Pi OS, Ubuntu, Debian

### 1. Install Python and OpenCV
```bash
sudo apt install -y python3-venv python3-opencv
```

### 2. Create and activate a virtual environment
```bash
python3 -m venv .env --system-site-packages
source .env/bin/activate
pip install --upgrade pip setuptools wheel
```

### 3. Download and install the Scanbot SDK
1. Go to [GitHub Releases](https://github.com/doo/scanbot-sdk-example-linux/releases)  
2. Manually download the **Python package** for your platform (ARM64 or x86_64).  
3. Install it with pip, for example:

* **On ARM64 (Raspberry Pi, Jetson Nano, etc.):**
  ```bash
  pip install ./scanbotsdk-<VERSION>-py3-none-linux_aarch64.whl
  ```

* **On x86_64 (Ubuntu/Debian Desktop, Servers):**
  ```bash
  pip install ./scanbotsdk-<VERSION>-py3-none-linux_x86_64.whl
  ```

### 4. Verify installation
```bash
python -c "import scanbotsdk"
```

✅ Done! You can now run the examples.

## API References
👉 [Scanbot SDK Python API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/python-api/)
