#!/bin/bash
set -e

echo "=== Node.js SDK Command Tests ==="

# Find the project root directory
if [[ -d "/workspaces/scanbot-sdk-example-linux/examples/nodejs" ]]; then
    cd /workspaces/scanbot-sdk-example-linux/examples/nodejs
elif [[ -d "examples/nodejs" ]]; then
    cd examples/nodejs
else
    echo "ERROR: Cannot find Node.js examples directory"
    exit 1
fi

echo "Testing TypeScript compilation..."
if npx tsc --noEmit >/dev/null 2>&1; then
    echo "PASS: TypeScript compilation: PASSED"
else
    echo "FAIL: TypeScript compilation: FAILED"
    exit 1
fi

# Check if license is available
if [[ -z "${SCANBOT_LICENSE}" ]]; then
    echo "ERROR: No license available"
    echo "SCANBOT_LICENSE environment variable is not set"
    echo "Tests cannot run without a valid license"
    exit 1
fi

echo "Testing NODEJS commands..."
commands=(
    "scan barcode --file ../images/qrcode.png --license \"${SCANBOT_LICENSE}\""
    "scan document --file ../images/Document.png --license \"${SCANBOT_LICENSE}\""
    "scan check --file ../images/check.jpg --license \"${SCANBOT_LICENSE}\""
    "scan credit_card --file ../images/credit_card.png --license \"${SCANBOT_LICENSE}\""
    "scan document_extractor --file ../images/Document.png --license \"${SCANBOT_LICENSE}\""
    "scan medical_certificate --file ../images/medical_certificate.png --license \"${SCANBOT_LICENSE}\""
    "scan mrz --file ../images/MRZ_passport.png --license \"${SCANBOT_LICENSE}\""
    "scan ocr --file ../images/Document.png --license \"${SCANBOT_LICENSE}\""
    "scan text_pattern --file ../images/Document.png --license \"${SCANBOT_LICENSE}\""
    "scan vin --file ../images/VIN.png --license \"${SCANBOT_LICENSE}\""
    "classify document --file ../images/toll_receipt.png --license \"${SCANBOT_LICENSE}\""
    "analyze analyze_multi_page --file ../images/multi_page_document.pdf --license \"${SCANBOT_LICENSE}\""
    "analyze crop_analyze --file ../images/Document.png --license \"${SCANBOT_LICENSE}\""
    "parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<<<<<<<<<<ERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<<<<<<<<<<\" --license \"${SCANBOT_LICENSE}\""
    "parse barcode_doc --text \"QR_CODE:https://example.com\" --license \"${SCANBOT_LICENSE}\""
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
    
    if timeout 30 npx ts-node src/index.ts $cmd; then
        echo "PASS: $name: PASSED"
    elif [[ $? -eq 124 ]]; then
        echo "FAIL: $name: TIMEOUT"
        exit 1
    else
        echo "FAIL: $name: FAILED"
        exit 1
    fi
done

echo "PASS: Node.js tests PASSED"
