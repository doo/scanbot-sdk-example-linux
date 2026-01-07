# Scanbot SDK – Node.js on Linux

### Install the Scanbot SDK
Replace `<SCANBOT_SDK_VERSION>` with the actual version number of the SDK you want to install.
```bash
export SCANBOT_SDK_VERSION=<SCANBOT_SDK_VERSION>
```

```bash
npm install https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv${SCANBOT_SDK_VERSION}/nodejs-scanbotsdk-${SCANBOT_SDK_VERSION}.tgz
```

### Verify installation
```bash
node -e "console.log(require('scanbotsdk') ? 'Scanbot SDK loaded' : 'Error')"
```

## Usage
The example supports four modes: **scan**, **analyze**, **classify**, and **parse**.
```bash
npx ts-node src/index.ts scan <subcommand> --file <path/to/file.jpg> [--license <KEY>]
npx ts-node src/index.ts scan <subcommand> --file <path/to/file.jpg> [--license <KEY>]
npx ts-node src/index.ts analyze <subcommand> --file <path/to/file.jpg> [--save <out.jpg>] [--license <KEY>]
npx ts-node src/index.ts analyze <subcommand> --file <path/to/file.jpg> [--save <out.jpg>] [--license <KEY>]
npx ts-node src/index.ts classify <subcommand> --file <path/to/file.jpg> [--license <KEY>]
npx ts-node src/index.ts parse <subcommand> --text "<input>" [--license <KEY>]
```

## Example
```bash
npx ts-node src/index.ts scan barcode --file images/example.jpg --license <KEY>
npx ts-node src/index.ts analyze analyze_multi_page --file files/doc.pdf --license <KEY>
npx ts-node src/index.ts analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>
npx ts-node src/index.ts parse mrz --text "P<UTOERIKSSON<<ANNA<MARIA<<<<<<" --license <KEY>
```

## API References
👉 [Scanbot SDK Node.js API](https://scanbotsdk.github.io/documentation/barcode-scanner-sdk/linux/nodejs-api/)