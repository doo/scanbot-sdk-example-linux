# Scanbot SDK – Node.js on Linux
This repository demonstrates how to use the [Scanbot SDK for Linux](https://scanbot.io) with Node.js.  

## Setup

### Install the Scanbot SDK
Replace `<SCANBOT_SDK_VERSION>` with the actual version number of the SDK you want to install.
  ```bash
  npm install https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv<SCANBOT_SDK_VERSION>/nodejs-scanbotsdk-<SCANBOT_SDK_VERSION>.tgz
  ```

### Verify installation
```bash
node -e "console.log(require('scanbotsdk') ? 'Scanbot SDK loaded' : 'Error')"
```

✅ You're ready to run the example!

## API References
👉 [Scanbot SDK Node.js API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/nodejs-api/)