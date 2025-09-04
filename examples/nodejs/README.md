# Scanbot SDK – Node.js on Linux
This repository demonstrates how to use the [Scanbot SDK for Linux](https://scanbot.io) with Node.js.  

## Setup

1. Go to [GitHub Releases](https://github.com/doo/scanbot-sdk-example-linux/releases)  
2. Manually download the correct archive for your platform:

* **On ARM64 (Raspberry Pi, Jetson, ARM servers):**  
  `nodejs_addon-scanbotsdk-<VERSION>-linux-aarch64.tar.gz`

* **On x86_64 (Desktop/Server Linux):**  
  `nodejs_addon-scanbotsdk-<VERSION>-linux-x86_64.tar.gz`

### 3. Extract the addon
```bash
tar -xzf nodejs_addon-scanbotsdk-<VERSION>-linux-<arch>.tar.gz
```

### 4. Install the addon into this project
From inside the `nodejs/` project folder:
```bash
npm install ./nodejs_addon-scanbotsdk-<VERSION>-linux-<arch>
```

### 5. Verify installation
```bash
node -e "console.log(require('scanbotsdk') ? 'Scanbot SDK loaded' : 'Error')"
```

✅ You're ready to run the example!

## API References
👉 [Scanbot SDK Node.js API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/nodejs-api/)