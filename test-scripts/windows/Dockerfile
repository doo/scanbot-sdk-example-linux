ARG BASE_IMAGE
FROM ${BASE_IMAGE} AS base

SHELL ["powershell", "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command"]

# Build arguments
ARG ARCH=windows-x86_64
ARG SDK_VERSION
ARG SCANBOT_LICENSE
ARG PYTHON_SDK_WHL_URL
ARG C_SDK_ARCHIVE_URL

# Environment
ENV ARCH=${ARCH} \
    SDK_VERSION=${SDK_VERSION} \
    C_SDK_ARCHIVE_URL=${C_SDK_ARCHIVE_URL} \
    SCANBOT_LICENSE=${SCANBOT_LICENSE} \
    PYTHON_SDK_WHL_URL=${PYTHON_SDK_WHL_URL} \
    VS_INSTALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools" \
    PYTHON_EXE="C:\Python-amd64\python.exe" \
    SDK_BASE_URL="https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv" \
    PATH="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;${PATH}" \
    PYTHONUTF8=1

# Configure Python dependencies using preinstalled interpreter from the base image
RUN $ErrorActionPreference = 'Stop'; \
  $pythonExe = $env:PYTHON_EXE; \
  if (-not (Test-Path $pythonExe)) { throw ('Expected Python executable not found: ' + $pythonExe) }; \
    & $pythonExe --version; \
    & $pythonExe -m pip install --upgrade pip setuptools wheel; \
    & $pythonExe -m pip install --no-cache-dir --no-deps 'opencv-python-headless==4.5.5.64'; \
    & $pythonExe -m pip install --no-cache-dir 'numpy==1.26.4' pillow;

# Set workspace path (source is copied later to improve layer caching)
WORKDIR C:/workspaces/scanbot-sdk-example-linux

# Install Scanbot Python SDK from either explicit URL or release URL computed from SDK_VERSION and ARCH
RUN $ErrorActionPreference = 'Stop'; \
  $pythonExe = $env:PYTHON_EXE; \
  if (-not (Test-Path $pythonExe)) { throw ('Expected Python executable not found: ' + $pythonExe) }; \
    if ($env:ARCH -in @('windows-arm64', 'win-arm64')) { $pythonArch = 'win_arm64' } else { $pythonArch = 'win_amd64' }; \
    if ([string]::IsNullOrWhiteSpace($env:PYTHON_SDK_WHL_URL)) { \
      $wheelUrl = ('{0}{1}/scanbotsdk-{1}-py3-none-{2}.whl' -f $env:SDK_BASE_URL, $env:SDK_VERSION, $pythonArch); \
    } else { \
      $wheelUrl = $env:PYTHON_SDK_WHL_URL; \
    }; \
    Write-Host ('Installing Python SDK from: {0}' -f $wheelUrl); \
    & $pythonExe -m pip install $wheelUrl; \
    Write-Host 'Python SDK installed successfully'

RUN $ErrorActionPreference = 'Stop'; \
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; \
    if ($env:ARCH -in @('windows-x86_64', 'windows-x64')) { $cSdkArch = 'windows-x64' } else { throw ('Unsupported architecture: ' + $env:ARCH) }; \
    if ([string]::IsNullOrWhiteSpace($env:C_SDK_ARCHIVE_URL)) { \
      $cSdkUrl = ('{0}{1}/scanbotsdk-{1}-{2}.tar.gz' -f $env:SDK_BASE_URL, $env:SDK_VERSION, $cSdkArch); \
    } else { \
      $cSdkUrl = $env:C_SDK_ARCHIVE_URL; \
    }; \
    $archivePath = 'C:\scanbotsdk-c.tar.gz'; \
    $sdkRoot = 'C:\scanbotsdk-cache'; \
    if (Test-Path $sdkRoot) { Remove-Item $sdkRoot -Recurse -Force }; \
    New-Item -ItemType Directory -Path $sdkRoot | Out-Null; \
    Write-Host ('Downloading C SDK from: {0}' -f $cSdkUrl); \
    Invoke-WebRequest -Uri $cSdkUrl -OutFile $archivePath; \
    Push-Location $sdkRoot; \
    & cmake -E tar -xf $archivePath; \
    Pop-Location; \
    Remove-Item $archivePath -Force; \
    if (-not (Test-Path (Join-Path $sdkRoot 'scanbotsdk\include\ScanbotSDK.h'))) { throw 'C SDK include files not found after extraction.' }; \
    if (-not (Test-Path (Join-Path $sdkRoot 'scanbotsdk\lib'))) { throw 'C SDK lib directory not found after extraction.' }; \
    Set-Content -Path C:\c-sdk-dir.txt -Value $sdkRoot -NoNewline; \
    Write-Host ('C SDK installed at: {0}' -f (Join-Path $sdkRoot 'scanbotsdk'))

# Build C examples with preinstalled VS Build Tools
COPY . .

RUN $ErrorActionPreference = 'Stop'; \
  $installPath = $env:VS_INSTALL_PATH; \
  if ([string]::IsNullOrWhiteSpace($installPath)) { throw 'VS install path environment variable is empty.' }; \
  $dq = [char]34; \
  $nl = [Environment]::NewLine; \
  $installPath = $installPath.Trim().Trim($dq); \
  $vsDevCmd = (Join-Path $installPath 'Common7\Tools\VsDevCmd.bat').Trim($dq); \
  if (-not (Test-Path $vsDevCmd)) { throw ('VsDevCmd.bat not found under: ' + $installPath) }; \
  $cSdkDir = (Get-Content C:\c-sdk-dir.txt -Raw).Trim(); \
  if ([string]::IsNullOrWhiteSpace($cSdkDir)) { throw 'C SDK dir is empty' }; \
  $cmdFile = 'C:\build-c-sdk.cmd'; \
  $cmdText = '@echo on' + $nl + \
             'setlocal EnableExtensions' + $nl + \
             ('call ' + $dq + $vsDevCmd + $dq + ' -arch=x64 || exit /b %errorlevel%') + $nl + \
             'cd /d C:\workspaces\scanbot-sdk-example-linux || exit /b %errorlevel%' + $nl + \
             ('cmake -S examples\c -B examples\c\build -G' + $dq + 'Visual Studio 17 2022' + $dq + ' -A x64 -DSCANBOTSDK_DIR=' + $dq + $cSdkDir + $dq + ' -DSCANBOTSDK_VERSION=' + $env:SDK_VERSION + ' || exit /b %errorlevel%') + $nl + \
             'cmake --build examples\c\build --config Release -- /m || exit /b %errorlevel%'; \
  [System.IO.File]::WriteAllText($cmdFile, $cmdText, [System.Text.Encoding]::ASCII); \
  & cmd.exe /d /c C:\build-c-sdk.cmd; \
  $exitCode = $LASTEXITCODE; \
  Remove-Item $cmdFile -Force -ErrorAction SilentlyContinue; \
  if ($exitCode -ne 0) { throw ('C build failed with exit code ' + $exitCode) };

# SDK verification stage
FROM base AS sdk-verification
RUN $ErrorActionPreference = 'Stop'; \
    Write-Host '=== Comprehensive SDK Verification ==='; \
    cmake --version; \
    if ($LASTEXITCODE -ne 0) { throw 'cmake not available' }; \
    $pythonExe = $env:PYTHON_EXE; \
    if (-not (Test-Path $pythonExe)) { throw ('Expected Python executable not found: ' + $pythonExe) }; \
    $pyCode = 'import scanbotsdk; print(''Python SDK: Verified'')'; \
    & $pythonExe -c $pyCode; \
    $pyExit = $LASTEXITCODE; \
    if ($pyExit -ne 0) { throw ('Python SDK verification failed with exit code ' + $pyExit) }; \
    $cSdkRoot = (Get-Content C:\c-sdk-dir.txt -Raw).Trim(); \
    if ([string]::IsNullOrWhiteSpace($cSdkRoot)) { throw 'C SDK dir file is empty' }; \
    if (-not (Test-Path (Join-Path $cSdkRoot 'scanbotsdk\include\ScanbotSDK.h'))) { throw 'C SDK header not found' }; \
    if (-not (Test-Path (Join-Path $cSdkRoot 'scanbotsdk\lib'))) { throw 'C SDK lib directory not found' }; \
    $exePath = 'C:\workspaces\scanbot-sdk-example-linux\examples\c\build\Release\scanbotsdk_example.exe'; \
    if ([string]::IsNullOrWhiteSpace($exePath)) { throw 'C example executable not found in build outputs' }; \
    Write-Host ('C SDK: Verified executable at {0}' -f $exePath)

# Python tests stage (PowerShell-only, no bash)
FROM sdk-verification AS python-tests
RUN $ErrorActionPreference = 'Stop'; \
    Write-Host '=== Running Python Command Tests ==='; \
    powershell -NoProfile -ExecutionPolicy Bypass -File C:/workspaces/scanbot-sdk-example-linux/test-scripts/windows/test-python.ps1

# C tests stage (PowerShell-only, no bash)
FROM sdk-verification AS c-tests
RUN $ErrorActionPreference = 'Stop'; \
    Write-Host '=== Running C Command Tests ==='; \
    powershell -NoProfile -ExecutionPolicy Bypass -File C:/workspaces/scanbot-sdk-example-linux/test-scripts/windows/test-c.ps1

# All tests stage (kept for target compatibility with existing workflow patterns)
FROM sdk-verification AS all-tests
RUN $ErrorActionPreference = 'Stop'; \
    powershell -NoProfile -ExecutionPolicy Bypass -File C:/workspaces/scanbot-sdk-example-linux/test-scripts/windows/run-all-tests.ps1
