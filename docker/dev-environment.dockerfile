FROM debian:13

WORKDIR /development

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends \
        # Core Compilers & Build Tools \
        build-essential \
        gcc \
        g++ \
        gdb \
        cmake \
        ninja-build \
        make \
        pkg-config \
        \
        # LSP Servers & LLVM/Clang Ecosystem \
        clang \
        clangd \
        clang-format \
        clang-tidy \
        lldb \
        \
        # Debugging, Memory Checking & Static Analysis \
        valgrind \
        cppcheck \
        \
        # Version Control & Utilities (often required by build scripts) \
        git \
        curl \
        wget \
        python3 \
        python3-pip \
        # Dependencies
        libasio-dev \
    && rm -rf /var/lib/apt/lists/*

RUN useradd cat-dev

CMD ["sleep", "inf"]
