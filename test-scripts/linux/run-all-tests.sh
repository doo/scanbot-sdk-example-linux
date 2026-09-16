#!/bin/bash
set -e

echo "========================================"
echo "    Scanbot SDK - Full Test Suite"
echo "========================================"

# Track test results
FAILED_TESTS=""
TOTAL_TESTS=0
PASSED_TESTS=0

run_test() {
    local test_name="$1"
    local test_script="$2"
    
    echo ""
    echo "Running $test_name tests..."
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if bash "$test_script"; then
        echo "PASS: $test_name tests PASSED"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo "✗ $test_name tests FAILED"
        FAILED_TESTS="$FAILED_TESTS $test_name"
    fi
}

# Run all test suites
run_test "Python" "/tests/test-python.sh"
run_test "Java" "/tests/test-java.sh"
run_test "Node.js" "/tests/test-nodejs.sh"
run_test "C" "/tests/test-c.sh"

# Final report
echo ""
echo "========================================"
echo "           Test Summary"
echo "========================================"
echo "Total test suites: $TOTAL_TESTS"
echo "Passed: $PASSED_TESTS"
echo "Failed: $((TOTAL_TESTS - PASSED_TESTS))"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo ""
    echo "ALL TESTS PASSED!"
    echo "Scanbot SDK is working correctly across all platforms."
    exit 0
else
    echo ""
    echo "Some tests failed:"
    echo "$FAILED_TESTS"
    echo ""
    echo "Please check the logs above for details."
    exit 1
fi
