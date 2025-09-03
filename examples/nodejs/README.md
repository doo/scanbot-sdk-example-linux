# Scanbot SDK – Node.js on Linux

📖 Full Node.js API documentation:  
[Scanbot SDK Node.js API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/nodejs-api/)

---

## Installation

### 1. Prerequisites
Make sure you have Node.js **>= 14** and npm installed:
```bash
node -v
npm -v
```

If not, install Node.js via your package manager or [NodeSource](https://github.com/nodesource/distributions).

### 2. Download the Scanbot SDK addon
1. Go to [GitHub Releases](https://github.com/doo/scanbot-sdk-example-linux/releases)  
2. Manually download the correct archive for your platform:

* **On ARM64 (Raspberry Pi, Jetson, ARM servers):**  
  `nodejs_addon-scanbotsdk-0.800.3-linux-aarch64.tar.gz`

* **On x86_64 (Desktop/Server Linux):**  
  `nodejs_addon-scanbotsdk-0.800.3-linux-x86_64.tar.gz`

### 3. Extract the addon
```bash
tar -xzf nodejs_addon-scanbotsdk-0.800.3-linux-<arch>.tar.gz
```

### 4. Install the addon into this project
From inside the `nodejs/` project folder:
```bash
npm install ./nodejs_addon-scanbotsdk-0.800.3-linux-<arch>
```

### 5. Verify installation
```bash
node -e "console.log(require('scanbotsdk') ? 'Scanbot SDK loaded' : 'Error')"
```

✅ You're ready to run the example!