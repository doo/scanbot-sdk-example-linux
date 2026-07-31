FROM mcr.microsoft.com/windows/servercore:ltsc2022 AS base

SHELL ["powershell", "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command"]

# Build arguments
ARG ARCH=windows-x86_64
ARG SDK_VERSION
ARG SCANBOT_LICENSE
ARG PYTHON_VERSION=3.10.11
ARG PYTHON_SDK_WHL_URL
ARG VS_BUILDTOOLS_CHANNEL=17
ARG C_SDK_ARCHIVE_URL

# Environment
ENV ARCH=${ARCH} \
    SDK_VERSION=${SDK_VERSION} \
    C_SDK_ARCHIVE_URL=${C_SDK_ARCHIVE_URL} \
    SCANBOT_LICENSE=${SCANBOT_LICENSE} \
    PYTHON_VERSION=${PYTHON_VERSION} \
    PYTHON_SDK_WHL_URL=${PYTHON_SDK_WHL_URL} \
    VS_BUILDTOOLS_CHANNEL=${VS_BUILDTOOLS_CHANNEL} \
    SDK_BASE_URL="https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv" \
    PYTHONUTF8=1

# Install Visual Studio C++ Build Tools, CMake and Ninja for C tests
RUN $ErrorActionPreference = 'Stop'; \
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; \
    $vsInstaller = 'C:\vs_BuildTools.exe'; \
    $vsUri = ('https://aka.ms/vs/{0}/release/vs_BuildTools.exe' -f $env:VS_BUILDTOOLS_CHANNEL); \
    Invoke-WebRequest -Uri $vsUri -OutFile $vsInstaller; \
    $vsArgs = @( \
      '--quiet', \
      '--wait', \
      '--norestart', \
      '--installPath', 'C:\BuildTools', \
      '--add', 'Microsoft.VisualStudio.Workload.VCTools', \
      '--add', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64', \
      '--add', 'Microsoft.VisualStudio.Component.Windows10SDK.19041', \
      '--includeRecommended' \
    ); \
    $proc = Start-Process -FilePath $vsInstaller -ArgumentList $vsArgs -Wait -PassThru; \
    $exitCode = $proc.ExitCode; \
    if (($exitCode -ne 0) -and ($exitCode -ne 3010)) { \
      Write-Host 'VS installer failed. Dumping VS temp logs:'; \
      Get-ChildItem -Path $env:TEMP -Filter 'dd_*' -ErrorAction SilentlyContinue | Sort-Object LastWriteTime -Descending | Select-Object -First 20 | ForEach-Object { Write-Host $_.FullName; Get-Content $_.FullName -Tail 80 }; \
      throw ('VS Build Tools install failed with exit code ' + $exitCode) \
    }; \
    $pf86 = [Environment]::GetFolderPath('ProgramFilesX86'); \
    $vsWhere = Join-Path $pf86 'Microsoft Visual Studio\Installer\vswhere.exe'; \
    if (-not (Test-Path $vsWhere)) { throw 'vswhere.exe not found after VS installer run.' }; \
    $installPath = (& $vsWhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath).Trim(); \
    if ([string]::IsNullOrWhiteSpace($installPath)) { throw 'VC Build Tools instance not found after installation.' }; \
    Set-Content -Path C:\vs-install-path.txt -Value $installPath -NoNewline; \
    Remove-Item $vsInstaller -Force

# Install Python
RUN $ErrorActionPreference = 'Stop'; \
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; \
    $pythonInstaller = 'C:\python-installer.exe'; \
    $pythonUrl = ('https://www.python.org/ftp/python/{0}/python-{0}-amd64.exe' -f $env:PYTHON_VERSION); \
    Invoke-WebRequest -Uri $pythonUrl -OutFile $pythonInstaller; \
    $proc = Start-Process -FilePath $pythonInstaller -ArgumentList '/quiet InstallAllUsers=1 PrependPath=1 Include_pip=1 Include_test=0 Include_launcher=1' -Wait -PassThru; \
    if ($proc.ExitCode -ne 0) { throw ('Python installer failed with exit code ' + $proc.ExitCode) }; \
    Remove-Item $pythonInstaller -Force; \
    $env:Path = [System.Environment]::GetEnvironmentVariable('Path','Machine') + ';' + [System.Environment]::GetEnvironmentVariable('Path','User'); \
    $pythonExe = (Get-Command python -ErrorAction SilentlyContinue).Source; \
    if (-not $pythonExe) { $pythonExe = (Get-Command py -ErrorAction Stop).Source }; \
    Set-Content -Path C:\python-exe.txt -Value $pythonExe -NoNewline; \
    & $pythonExe --version; \
    & $pythonExe -m pip install --upgrade pip setuptools wheel; \
    & $pythonExe -m pip uninstall -y opencv-python opencv-python-headless numpy; \
    & $pythonExe -m pip install --no-cache-dir 'numpy==1.26.4' pillow cmake ninja; \
    & $pythonExe -m pip install --no-cache-dir --no-deps 'opencv-python-headless==4.5.5.64'; \
    & $pythonExe -c 'import numpy, cv2; print("numpy", numpy.__version__); print("cv2", cv2.__version__)';  \
    cmake --version; \
    ninja --version

# Copy source
WORKDIR C:/workspaces/scanbot-sdk-example-linux
COPY . .

# Install Scanbot Python SDK from either explicit URL or release URL computed from SDK_VERSION and ARCH
RUN $ErrorActionPreference = 'Stop'; \
    $pythonExe = (Get-Content C:\python-exe.txt -Raw).Trim(); \
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
    if ($env:ARCH -in @('windows-x86_64', 'windows-x64')) { $cSdkArch = 'windows-x64' } elseif ($env:ARCH -in @('windows-arm64', 'win-arm64')) { $cSdkArch = 'windows-arm64' } else { throw ('Unsupported ARCH for C SDK: ' + $env:ARCH) }; \
    if ([string]::IsNullOrWhiteSpace($env:C_SDK_ARCHIVE_URL)) { \
      $cSdkUrl = ('{0}{1}/scanbotsdk-{1}-{2}.tar.gz' -f $env:SDK_BASE_URL, $env:SDK_VERSION, $cSdkArch); \
    } else { \
      $cSdkUrl = $env:C_SDK_ARCHIVE_URL; \
    }; \
    $archiveName = ('scanbotsdk-{0}-{1}.tar.gz' -f $env:SDK_VERSION, $cSdkArch); \
    $archivePath = 'C:\scanbotsdk-c.tar.gz'; \
    $sdkRoot = 'C:\scanbotsdk-cache'; \
    if (Test-Path $sdkRoot) { Remove-Item $sdkRoot -Recurse -Force }; \
    New-Item -ItemType Directory -Path $sdkRoot | Out-Null; \
    Write-Host ('Downloading C SDK from: {0}' -f $cSdkUrl); \
    Invoke-WebRequest -Uri $cSdkUrl -OutFile $archivePath; \
    Push-Location $sdkRoot; \
    cmake -E tar -xf $archivePath; \
    Pop-Location; \
    Remove-Item $archivePath -Force; \
    if (-not (Test-Path (Join-Path $sdkRoot 'scanbotsdk\include\ScanbotSDK.h'))) { throw 'C SDK include files not found after extraction.' }; \
    if (-not (Test-Path (Join-Path $sdkRoot 'scanbotsdk\lib'))) { throw 'C SDK lib directory not found after extraction.' }; \
    Set-Content -Path C:\c-sdk-dir.txt -Value $sdkRoot -NoNewline; \
    Write-Host ('C SDK installed at: {0}' -f (Join-Path $sdkRoot 'scanbotsdk'))

# Install CMake and Ninja for C tests
RUN $ErrorActionPreference = 'Stop'; \
    $installPath = (Get-Content C:\vs-install-path.txt -Raw).Trim(); \
    $vsDevCmd = Join-Path $installPath 'Common7\Tools\VsDevCmd.bat'; \
    $cSdkDir = (Get-Content C:\c-sdk-dir.txt -Raw).Trim(); \
    if ([string]::IsNullOrWhiteSpace($cSdkDir)) { throw 'C SDK dir is empty' }; \
    cmd /c ('"' + $vsDevCmd + '" -arch=x64 && cd /d C:\workspaces\scanbot-sdk-example-linux && cmake -S examples\c -B examples\c\build -G Ninja -DSCANBOTSDK_DIR=' + $cSdkDir + ' -DSCANBOTSDK_VERSION=' + $env:SDK_VERSION + ' && cmake --build examples\c\build --config Release -- -k 50'); \
    if ($LASTEXITCODE -ne 0) { throw ('C build failed with exit code ' + $LASTEXITCODE) };

# SDK verification stage
FROM base AS sdk-verification
RUN $ErrorActionPreference = 'Stop'; \
    Write-Host '=== Comprehensive SDK Verification ==='; \
    $installPath = (Get-Content C:\vs-install-path.txt -Raw).Trim(); \
    if ([string]::IsNullOrWhiteSpace($installPath)) { throw 'VS install path not found.' }; \
    $vsDevCmd = Join-Path $installPath 'Common7\Tools\VsDevCmd.bat'; \
    $vcvars64 = Join-Path $installPath 'VC\Auxiliary\Build\vcvars64.bat'; \
    if (Test-Path $vsDevCmd) { \
      cmd /c ('"' + $vsDevCmd + '" -arch=x64 && where cl && cl /? >nul'); \
    } elseif (Test-Path $vcvars64) { \
      cmd /c ('"' + $vcvars64 + '" && where cl && cl /? >nul'); \
    } else { \
      throw ('Neither VsDevCmd.bat nor vcvars64.bat found under: ' + $installPath) \
    }; \
    if ($LASTEXITCODE -ne 0) { throw ('MSVC toolchain verification failed with exit code ' + $LASTEXITCODE) }; \
    cmake --version; \
    if ($LASTEXITCODE -ne 0) { throw 'cmake not available' }; \
    ninja --version; \
    if ($LASTEXITCODE -ne 0) { throw 'ninja not available' }; \
    $pythonExe = (Get-Content C:\python-exe.txt -Raw).Trim(); \
    $pyCode = 'import scanbotsdk; print(''Python SDK: Verified'')'; \
    & $pythonExe -c $pyCode; \
    $pyExit = $LASTEXITCODE; \
    if ($pyExit -ne 0) { throw ('Python SDK verification failed with exit code ' + $pyExit) };

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
