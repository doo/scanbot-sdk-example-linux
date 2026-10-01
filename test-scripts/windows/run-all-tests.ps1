$ErrorActionPreference = 'Stop'

Write-Host '========================================'
Write-Host '    Scanbot SDK - Full Test Suite'
Write-Host '========================================'

$failedTests = @()
$totalTests = 0
$passedTests = 0

function Invoke-TestSuite {
    param(
        [Parameter(Mandatory = $true)][string]$TestName,
        [Parameter(Mandatory = $true)][string]$ScriptPath
    )

    Write-Host ''
    Write-Host ("Running {0} tests..." -f $TestName)
    $script:totalTests += 1

    if (-not (Test-Path -Path $ScriptPath)) {
        Write-Host ("FAIL: {0} tests FAILED (script not found: {1})" -f $TestName, $ScriptPath)
        $script:failedTests += $TestName
        return
    }

    try {
        & powershell -NoProfile -ExecutionPolicy Bypass -File $ScriptPath
        if ($LASTEXITCODE -eq 0) {
            Write-Host ("PASS: {0} tests PASSED" -f $TestName)
            $script:passedTests += 1
        } else {
            Write-Host ("FAIL: {0} tests FAILED" -f $TestName)
            $script:failedTests += $TestName
        }
    } catch {
        Write-Host ("FAIL: {0} tests FAILED" -f $TestName)
        Write-Host $_.Exception.Message
        $script:failedTests += $TestName
    }
}

$pythonScript = Join-Path $PSScriptRoot 'test-python.ps1'
$cScript = Join-Path $PSScriptRoot 'test-c.ps1'

Invoke-TestSuite -TestName 'Python' -ScriptPath $pythonScript
Invoke-TestSuite -TestName 'C' -ScriptPath $cScript

Write-Host ''
Write-Host '========================================'
Write-Host '           Test Summary'
Write-Host '========================================'
Write-Host ("Total test suites: {0}" -f $totalTests)
Write-Host ("Passed: {0}" -f $passedTests)
Write-Host ("Failed: {0}" -f ($totalTests - $passedTests))

if ($passedTests -eq $totalTests) {
    Write-Host ''
    Write-Host 'ALL TESTS PASSED!'
    Write-Host 'Scanbot SDK is working correctly across all platforms.'
    exit 0
}

Write-Host ''
Write-Host 'Some tests failed:'
Write-Host ($failedTests -join ', ')
Write-Host ''
Write-Host 'Please check the logs above for details.'
exit 1
