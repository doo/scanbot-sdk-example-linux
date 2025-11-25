# Test Scripts

These test scripts validate the Scanbot SDK integration across all supported languages and detect common issues like licensing checks and compilation errors.

## What Tests Do

- **Check compilation**: Verify code builds correctly
- **Validate license**: Ensure license key works  
- **Test commands**: Run SDK functions to catch integration issues
- **Quick validation**: Fast checks for development workflow

**Note: No asserts are setup to validate the tests results.**

## Test Structure

```
test-scripts/
├── test-python.sh     # Python SDK tests
├── test-java.sh       # Java SDK tests  
├── test-nodejs.sh     # Node.js SDK tests
├── test-c.sh          # C SDK tests
├── run-all-tests.sh   # Run all tests
└── test-images/       # Test image files
```

## Running Tests

### 1. Build Test Container

```bash
docker build \
    --build-arg SDK_VERSION=0.800.3 \
    --build-arg ARCH=linux-aarch64 \
    --build-arg SCANBOT_LICENSE=$SCANBOT_LICENSE \
    --target sdk-verification \
    -t scanbot-test .
```

### 2. Set License Key

```bash
export SCANBOT_LICENSE="your-license-key-here"
```

### 3. Run Tests

**Single test:**
```bash
docker run --rm \
    --env SCANBOT_LICENSE=$SCANBOT_LICENSE \
    --workdir /workspaces/scanbot-sdk-example-linux \
    scanbot-test \
    /tests/test-python.sh
```

**All tests:**
```bash
docker run --rm \
    --env SCANBOT_LICENSE=$SCANBOT_LICENSE \
    --workdir /workspaces/scanbot-sdk-example-linux \
    scanbot-test \
    /tests/run-all-tests.sh
```

## Test Commands

Each test validates these SDK functions:

- **Barcode scanning** - QR codes, barcodes
- **Document scanning** - Document detection  
- **Data extraction** - Credit cards, checks, MRZ, VIN
- **OCR** - Text recognition
- **Document analysis** - Multi-page, cropping
- **Text parsing** - MRZ, barcode document parsing
