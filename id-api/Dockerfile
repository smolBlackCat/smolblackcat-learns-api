FROM debian:13 AS build

COPY ./ /development/
WORKDIR /development

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends \
        # Core Compilers & Build Tools \
        build-essential \
        gcc \
        g++ \
        cmake \
        ninja-build \
        make \
        pkg-config \
        # Version Control & Utilities
        git \
        curl \
        wget \
        python3 \
        python3-pip \
        # Dependencies
        libasio-dev \
        libmariadb-dev \
        libgnutls28-dev  \  
    && rm -rf /var/lib/apt/lists/*

# Build Steps
RUN cmake -S . -B build -GNinja -DENABLE_TEST=OFF -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build

FROM debian:13 AS runtime

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends \
    libssl3t64 \
    libzstd1 \
    zlib1g

COPY --from=build /development/build/gov-api /app/

CMD app/gov-api
