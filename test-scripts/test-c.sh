#!/bin/bash
set -e

echo "=== C SDK Command Tests ==="

# Find the project root directory
if [[ -d "/workspaces/scanbot-sdk-example-linux/examples/c" ]]; then
    cd /workspaces/scanbot-sdk-example-linux/examples/c
elif [[ -d "examples/c" ]]; then
    cd examples/c
else
    echo "ERROR: Cannot find C examples directory"
    exit 1
fi

# Test basic build first
echo "Testing C compilation..."
mkdir -p build
cd build
if cmake -DSCANBOTSDK_VERSION=0.800.3 .. >/dev/null 2>&1 && make >/dev/null 2>&1; then
    echo "PASS: C compilation: PASSED"
else
    echo "FAIL: C compilation: FAILED"
    exit 1
fi

echo "Testing executable exists..."
if [[ -x "./scanbotsdk_example" ]]; then
    echo "PASS: C executable: EXISTS"
    ls -la scanbotsdk_example
else
    echo "FAIL: C executable: NOT FOUND"
    exit 1
fi

# Check if license is available
if [[ -z "${SCANBOT_LICENSE}" ]]; then
    echo "ERROR: No license available"
    echo "SCANBOT_LICENSE environment variable is not set"
    echo "Tests cannot run without a valid license"
    exit 1
fi

LICENSE_KEY="${SCANBOT_LICENSE}"

echo "Testing SCAN commands..."
commands=(
    "scan barcode --file ../../images/qrcode.png --license \"${LICENSE_KEY}\""
    "scan document --file ../../images/Document.png --license \"${LICENSE_KEY}\""
    "scan check --file ../../images/check.jpg --license \"${LICENSE_KEY}\""
    "scan credit_card --file ../../images/credit_card.png --license \"${LICENSE_KEY}\""
    "scan document_extractor --file ../../images/Document.png --license \"${LICENSE_KEY}\""
    "scan medical_certificate --file ../../images/medical_certificate.png --license \"${LICENSE_KEY}\""
    "scan mrz --file ../../images/MRZ_passport.png --license \"${LICENSE_KEY}\""
    "scan ocr --file ../../images/Document.png --license \"${LICENSE_KEY}\""
    "scan text_pattern --file ../../images/Document.png --license \"${LICENSE_KEY}\""
    "scan vin --file ../../images/VIN.png --license \"${LICENSE_KEY}\""
    "classify document --file ../../images/toll_receipt.png --license \"${LICENSE_KEY}\""
    "analyze analyze_multi_page --file ../../images/multi_page_document.pdf --license \"${LICENSE_KEY}\""
    "analyze crop_analyze --file ../../images/Document.png --license \"${LICENSE_KEY}\""
    "parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<<<<<<<<<<ERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<<<<<<<<<<\" --license \"${LICENSE_KEY}\""
    "parse barcode_doc --text \"QR_CODE:https://example.com\" --license \"${LICENSE_KEY}\""
)

command_names=(
    "Barcode scan"
    "Document scan"
    "Check scan"
    "Credit card scan"
    "Document extractor scan"
    "Medical certificate scan"
    "MRZ scan"
    "OCR scan"
    "Text pattern scan"
    "VIN scan"
    "Document classify"
    "Multi-page analyze"
    "Crop analyze"
    "MRZ parse"
    "Barcode document parse"
)

for i in "${!commands[@]}"; do
    cmd="${commands[$i]}"
    name="${command_names[$i]}"
    
    if timeout 30 ./scanbotsdk_example $cmd >/dev/null 2>&1; then
        echo "PASS: $name: PASSED"
    elif [[ $? -eq 124 ]]; then
        echo "FAIL: $name: TIMEOUT"
        exit 1
    else
        echo "FAIL: $name: FAILED"
        exit 1
    fi
done

echo "PASS: C tests PASSED"
