# Scanbot SDK – Python on Linux

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

### 3. Install the Scanbot SDK
Replace `<SCANBOT_SDK_VERSION>` with the actual version number of the SDK you want to install.

```bash
export SCANBOT_SDK_VERSION=<SCANBOT_SDK_VERSION>
```

* **On ARM64 (Raspberry Pi, Jetson Nano, etc.):**
  ```bash
  pip install https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv${SCANBOT_SDK_VERSION}/scanbotsdk-${SCANBOT_SDK_VERSION}-py3-none-linux_aarch64.whl
  ```

* **On x86_64 (Ubuntu/Debian Desktop, Servers):**
  ```bash
  pip install https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv${SCANBOT_SDK_VERSION}/scanbotsdk-${SCANBOT_SDK_VERSION}-py3-none-linux_x86_64.whl
  ```

### 5. Verify installation
```bash
python -c "import scanbotsdk"
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

### 3. Install the Scanbot SDK
Replace `<SCANBOT_SDK_VERSION>` with the actual version number of the SDK you want to install.

```bash
export SCANBOT_SDK_VERSION=<SCANBOT_SDK_VERSION>
```

* **On ARM64 (Raspberry Pi, Jetson Nano, etc.):**
  ```bash
  pip install https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv${SCANBOT_SDK_VERSION}/scanbotsdk-${SCANBOT_SDK_VERSION}-py3-none-linux_aarch64.whl
  ```

* **On x86_64 (Ubuntu/Debian Desktop, Servers):**
  ```bash
  pip install https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv${SCANBOT_SDK_VERSION}/scanbotsdk-${SCANBOT_SDK_VERSION}-py3-none-linux_x86_64.whl
  ```

### 4. Verify installation
```bash
python -c "import scanbotsdk"
```

## Usage
The example supports four modes: **scan**, **analyze**, **classify**, and **parse**.
```bash
python main.py scan <subcommand> --file <path/to/file.jpg> [--license <KEY>]
python main.py scan <subcommand> --file <path/to/file.jpg> [--license <KEY>]
python main.py analyze <subcommand> --file <path/to/file.jpg> [--save <out.jpg>] [--license <KEY>]
python main.py analyze <subcommand> --file <path/to/file.jpg> [--save <out.jpg>] [--license <KEY>]
python main.py classify <subcommand> --file|--resource <path/to/file.jpg> [--license <KEY>]
python main.py parse <subcommand> --text "<input>" [--license <KEY>]
python main.py live <subcommand> --device "<device-input>" [--license <KEY>] [--preview]
```

## Example
```bash
python main.py scan barcode --file images/example.jpg --license <KEY>
python main.py analyze analyze_multi_page --file files/doc.pdf --license <KEY>
python main.py analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>
python main.py parse mrz --text "P<UTOERIKSSON<<ANNA<MARIA<<<<<<" --license <KEY>
python main.py live barcode --device "0" --license <KEY>
```

## API References
👉 [Scanbot SDK Python API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/python-api/)
