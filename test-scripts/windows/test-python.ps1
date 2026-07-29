$ErrorActionPreference = 'Stop'

Write-Host '=== Python SDK Command Tests ==='

$repoCandidates = @(
    'C:/workspaces/scanbot-sdk-example-linux',
    (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
)

$repoRoot = $null
foreach ($candidate in $repoCandidates) {
    if (Test-Path (Join-Path $candidate 'examples/python/main.py')) {
        $repoRoot = $candidate
        break
    }
}

if (-not $repoRoot) {
    Write-Host 'ERROR: Cannot find Python examples directory'
    exit 1
}

if ([string]::IsNullOrWhiteSpace($env:SCANBOT_LICENSE)) {
    Write-Host 'ERROR: No license available'
    Write-Host 'SCANBOT_LICENSE environment variable is not set'
    Write-Host 'Tests cannot run without a valid license'
    exit 1
}

Set-Location (Join-Path $repoRoot 'examples/python')

function Invoke-WithTimeout {
    param(
        [Parameter(Mandatory = $true)][string]$FilePath,
        [Parameter(Mandatory = $true)][string[]]$ArgumentList,
        [int]$TimeoutSec = 30
    )

    $proc = Start-Process -FilePath $FilePath -ArgumentList $ArgumentList -NoNewWindow -PassThru
    $null = Wait-Process -Id $proc.Id -Timeout $TimeoutSec -ErrorAction SilentlyContinue

    if (-not $proc.HasExited) {
        Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
        return 124
    }

    $proc.Refresh()
    return [int]$proc.ExitCode
}

Write-Host 'Testing SCAN commands...'

$commands = @(
    @{ Name = 'Barcode scan'; Args = @('main.py', 'scan', 'barcode', '--file', '../../test-scripts/test-images/qrcode.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Document scan'; Args = @('main.py', 'scan', 'document', '--file', '../../test-scripts/test-images/Document.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Check scan'; Args = @('main.py', 'scan', 'check', '--file', '../../test-scripts/test-images/check.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Credit card scan'; Args = @('main.py', 'scan', 'credit_card', '--file', '../../test-scripts/test-images/credit_card.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Document data extractor scan'; Args = @('main.py', 'scan', 'document_data_extractor', '--file', '../../test-scripts/test-images/EHIC.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Medical certificate scan'; Args = @('main.py', 'scan', 'medical_certificate', '--file', '../../test-scripts/test-images/medical_certificate.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'MRZ scan'; Args = @('main.py', 'scan', 'mrz', '--file', '../../test-scripts/test-images/MRZ_passport.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'OCR scan'; Args = @('main.py', 'scan', 'ocr', '--file', '../../test-scripts/test-images/Document.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Text pattern scan'; Args = @('main.py', 'scan', 'text_pattern', '--file', '../../test-scripts/test-images/Document.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'VIN scan'; Args = @('main.py', 'scan', 'vin', '--file', '../../test-scripts/test-images/VIN.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Document enhance'; Args = @('main.py', 'enhance', 'document', '--file', '../../test-scripts/test-images/Document.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Multi-page analyze'; Args = @('main.py', 'analyze', 'analyze_multi_page', '--file', '../../test-scripts/test-images/multi_page_document.pdf', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Crop analyze'; Args = @('main.py', 'analyze', 'crop_analyze', '--file', '../../test-scripts/test-images/Document.jpeg', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'MRZ parse'; Args = @('main.py', 'parse', 'mrz', '--text', 'P<UTOERIKSSON<<ANNA<MARIA<<<<<<<<<<<<<<<<<<<L898902C36UTO7408122F1204159ZE184226B<<<<<10', '--license', $env:SCANBOT_LICENSE) },
    @{ Name = 'Barcode document parse'; Args = @('main.py', 'parse', 'barcode_doc', '--text', '(01)03453120000011(17)191125(10)ABCD1234', '--license', $env:SCANBOT_LICENSE) }
)

foreach ($entry in $commands) {
    $exitCode = Invoke-WithTimeout -FilePath 'python' -ArgumentList $entry.Args -TimeoutSec 30
    if ($exitCode -eq 0) {
        Write-Host ("PASS: {0}: PASSED" -f $entry.Name)
    } elseif ($exitCode -eq 124) {
        Write-Host ("FAIL: {0}: TIMEOUT" -f $entry.Name)
        exit 1
    } else {
        Write-Host ("FAIL: {0}: FAILED" -f $entry.Name)
        exit 1
    }
}

Write-Host 'PASS: Python tests PASSED'

