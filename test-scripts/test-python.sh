#!/bin/bash
set -e

echo "=== Python SDK Command Tests ==="

# Find the project root directory
if [[ -d "/workspaces/scanbot-sdk-example-linux/examples/python" ]]; then
    cd /workspaces/scanbot-sdk-example-linux/examples/python
elif [[ -d "examples/python" ]]; then
    cd examples/python
else
    echo "ERROR: Cannot find Python examples directory"
    exit 1
fi

# Check if license is available
if [[ -z "${SCANBOT_LICENSE}" ]]; then
    echo "ERROR: No license available"
    echo "SCANBOT_LICENSE environment variable is not set"
    echo "Tests cannot run without a valid license"
    exit 1
fi

echo "Testing SCAN commands..."
commands=(
    "scan barcode --file ../../test-scripts/test-images/qrcode.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan document --file ../../test-scripts/test-images/Document.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan check --file ../../test-scripts/test-images/check.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan credit_card --file ../../test-scripts/test-images/credit_card.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan document_data_extractor --file ../../test-scripts/test-images/EHIC.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan medical_certificate --file ../../test-scripts/test-images/medical_certificate.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan mrz --file ../../test-scripts/test-images/MRZ_passport.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan ocr --file ../../test-scripts/test-images/Document.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan text_pattern --file ../../test-scripts/test-images/Document.jpeg --license \"${SCANBOT_LICENSE}\""
    "scan vin --file ../../test-scripts/test-images/VIN.jpeg --license \"${SCANBOT_LICENSE}\""
    "classify document --file ../../test-scripts/test-images/toll_receipt.jpeg --license \"${SCANBOT_LICENSE}\""
    "analyze analyze_multi_page --file ../../test-scripts/test-images/multi_page_document.pdf --license \"${SCANBOT_LICENSE}\""
    "analyze crop_analyze --file ../../test-scripts/test-images/Document.jpeg --license \"${SCANBOT_LICENSE}\""
    "parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<L898902C36UTO7408122F1204159ZE184226B<<<<<10\" --license \"${SCANBOT_LICENSE}\""
    "parse barcode_doc --text \"(01)03453120000011(17)191125(10)ABCD1234\" --license \"${SCANBOT_LICENSE}\""
)

command_names=(
    "Barcode scan"
    "Document scan"
    "Check scan"
    "Credit card scan"
    "Document data extractor scan"
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

    if timeout 30 python3 main.py $cmd; then
        echo "PASS: $name: PASSED"
    elif [[ $? -eq 124 ]]; then
        echo "FAIL: $name: TIMEOUT"
        exit 1
    else
        echo "FAIL: $name: FAILED"
        exit 1
    fi
done

echo "PASS: Python tests PASSED"

