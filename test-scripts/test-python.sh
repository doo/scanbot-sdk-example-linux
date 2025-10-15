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

# Test basic functionality first
echo "Testing Python imports..."
if python3 -c "import scanbotsdk; print('Scanbot SDK imported successfully')" 2>/dev/null; then
    echo "PASS: Python SDK import: PASSED"
else
    echo "FAIL: Python SDK import: FAILED"
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
# All failures are actual failures - no exceptions for license errors
commands=(
    "scan barcode --file ../images/qrcode.png --license \"${LICENSE_KEY}\""
    "scan document --file ../images/Document.JPG --license \"${LICENSE_KEY}\""
    "scan check --file ../images/check.jpg --license \"${LICENSE_KEY}\""
    "scan credit_card --file ../images/credit_card.png --license \"${LICENSE_KEY}\""
    "scan mrz --file \"../images/MEXico MRZ passport old-1.jpg\" --license \"${LICENSE_KEY}\""
    "scan vin --file ../images/VIN.png --license \"${LICENSE_KEY}\""
    "classify document --file ../images/Document.JPG --license \"${LICENSE_KEY}\""
    "analyze analyze_multi_page --file ../images/multi_page_document.pdf --license \"${LICENSE_KEY}\""
    "analyze crop_analyze --file ../images/Document.JPG --license \"${LICENSE_KEY}\""
    "parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<<<<<<<<<<ERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<<<<<<<<<<\" --license \"${LICENSE_KEY}\""
)

command_names=(
    "Barcode scan"
    "Document scan"
    "Check scan"
    "Credit card scan"
    "MRZ scan"
    "VIN scan"
    "Document classify"
    "Multi-page analyze"
    "Crop analyze"
    "MRZ parse"
)

for i in "${!commands[@]}"; do
    cmd="${commands[$i]}"
    name="${command_names[$i]}"
    
    if eval "timeout 30 python3 main.py $cmd" >/dev/null 2>&1; then
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
