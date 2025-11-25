
FROM debian:bookworm-slim AS base 

# Build Arguments
ARG ARCH
ARG SDK_VERSION
ARG JAVA_VERSION=17
ARG SCANBOT_LICENSE

# Environment Variables
ENV ARCH=${ARCH} \
    SDK_VERSION=${SDK_VERSION} \
    JAVA_VERSION=${JAVA_VERSION} \
    PYENV_ROOT="/opt/pyenv" \
    PATH="/opt/pyenv/bin:/opt/pyenv/shims:$PATH" \
    SDK_BASE_URL="https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv" \
    SCANBOT_LICENSE=${SCANBOT_LICENSE}

# Install system dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    # Build tools
    build-essential \
    cmake \
    make \
    libssl-dev \
    zlib1g-dev \
    libbz2-dev \
    libreadline-dev \
    libsqlite3-dev \
    libffi-dev \
    liblzma-dev \
    # Languages  
    openjdk-${JAVA_VERSION}-jdk \
    # Utilities
    curl \
    git \
    ca-certificates \
    # Add Node.js repository and install
    && curl -fsSL https://deb.nodesource.com/setup_lts.x | bash - \
    && apt-get install -y nodejs \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

    # Install pyenv and Python 3.6.15
RUN git clone --depth=1 https://github.com/pyenv/pyenv.git $PYENV_ROOT \
    && pyenv install 3.6.15 \
    && pyenv global 3.6.15 \
    && pyenv rehash

# Set JAVA_HOME
RUN export JAVA_HOME=$(readlink -f /usr/bin/javac | sed "s:/bin/javac::") && \
    echo "JAVA_HOME=$JAVA_HOME" >> /etc/environment

ENV PATH="/opt/venv/bin:${PATH}"

# Verify Java installation
RUN java -version && javac -version

# Set up Python packages
RUN export PATH="/opt/pyenv/bin:/opt/pyenv/shims:$PATH" \
    && eval "$(/opt/pyenv/bin/pyenv init -)" \
    && python -m pip install --upgrade pip setuptools wheel \
    # The opencv-python version is specified to ensure compatibility with Python 3.6 and speed up docker builds
    # Once the python is upgraded to 3.7+, this can be changed to just `opencv-python`
    && python -m pip install opencv-python==4.5.5.64 numpy pillow    

# Install Python SDK
RUN if [ "${ARCH}" = "linux-aarch64" ]; then \
        PYTHON_ARCH="linux_aarch64"; \
        SDK_ARCH="linux-aarch64"; \
    else \
        PYTHON_ARCH="linux_x86_64"; \
        SDK_ARCH="linux-x86_64"; \
    fi && \
    python -m pip install "${SDK_BASE_URL}${SDK_VERSION}/scanbotsdk-${SDK_VERSION}-py3-none-${PYTHON_ARCH}.whl" && \
    echo "Python SDK installed successfully"

# Set working directory and copy source code
WORKDIR /workspaces/scanbot-sdk-example-linux
COPY . .

# Download and install all remaining SDKs in optimal locations
RUN echo "Installing Java and C SDKs for architecture: ${ARCH}" && \
    # Set the correct SDK architecture for downloads
    if [ "${ARCH}" = "linux-aarch64" ]; then \
        SDK_ARCH="linux-aarch64"; \
    else \
        SDK_ARCH="linux-x86_64"; \
    fi && \
    # Download platform-dependent SDKs (Java and C only)
    curl -L -O "${SDK_BASE_URL}${SDK_VERSION}/scanbotsdk-${SDK_VERSION}-${SDK_ARCH}.jar" && \
    curl -L -O "${SDK_BASE_URL}${SDK_VERSION}/scanbotsdk-${SDK_VERSION}-${SDK_ARCH}.tar.gz" && \
    # Install Node.js SDK (platform-independent npm package)
    cd examples/nodejs && \
    npm install "${SDK_BASE_URL}${SDK_VERSION}/nodejs-scanbotsdk-${SDK_VERSION}.tgz" && \
    cd /workspaces/scanbot-sdk-example-linux && \
    # Setup Java SDK  
    mkdir -p examples/java/build/libs && \
    cp "scanbotsdk-${SDK_VERSION}-${SDK_ARCH}.jar" examples/java/build/libs/scanbotsdk.jar && \
    # Setup C SDK
    mkdir -p examples/c/build/scanbotsdk && \
    tar -xzf "scanbotsdk-${SDK_VERSION}-${SDK_ARCH}.tar.gz" -C examples/c/build/scanbotsdk --strip-components=1 && \
    # Clean up downloads
    rm -f *.tar.gz *.jar && \
    echo "All SDKs installed successfully"

# Copy test scripts
COPY test-scripts/ /tests/
RUN chmod +x /tests/*.sh

# SDK Verification Stage
FROM base AS sdk-verification
RUN echo "=== Comprehensive SDK Verification ===" \
    && python -c "import scanbotsdk; print('Python SDK: Verified')" \
    && cd examples/nodejs && npm install && node -e "const sdk = require('scanbotsdk'); console.log(sdk ? 'OK' : 'FAIL');" \
    && cd /workspaces/scanbot-sdk-example-linux/examples/java && ./gradlew check --no-daemon && echo "Java SDK: Verified" \
    && cd /workspaces/scanbot-sdk-example-linux/examples/c && mkdir -p build && cd build && cmake -DSCANBOTSDK_VERSION=${SDK_VERSION} .. && make && echo "C SDK: OK"

# Python Tests Stage
FROM sdk-verification AS python-tests
RUN echo "=== Running Python Command Tests ===" \
    && /tests/test-python.sh

# Java Tests Stage
FROM sdk-verification AS java-tests
RUN echo "=== Running Java Command Tests ===" \
    && /tests/test-java.sh

# Node.js Tests Stage
FROM sdk-verification AS nodejs-tests
RUN echo "=== Running Node.js Command Tests ===" \
    && /tests/test-nodejs.sh

# C Tests Stage
FROM sdk-verification AS c-tests
RUN echo "=== Running C Command Tests ===" \
    && /tests/test-c.sh

# All Tests Stage
FROM sdk-verification AS all-tests
RUN echo "=== Running Complete Test Suite ===" \
    && /tests/run-all-tests.sh \
    && echo "Python import and commands verified" \
    && echo "Java compilation and commands verified" \
    && echo "Node.js compilation and commands verified" \
    && echo "C compilation and commands verified"