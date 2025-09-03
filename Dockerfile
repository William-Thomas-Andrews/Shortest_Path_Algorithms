FROM ubuntu:22.04

# Install system dependencies (cached until Dockerfile changes)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    libssl-dev \
    libboost-all-dev \
    libevent-dev \
    libdouble-conversion-dev \
    libgoogle-glog-dev \
    libgflags-dev \
    libiberty-dev \
    liblz4-dev \
    liblzma-dev \
    libsnappy-dev \
    libzstd-dev \
    libunwind-dev \
    libjemalloc-dev \
    libfmt-dev \
    zlib1g-dev \
    graphviz \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Install FastFloat 8.0.2
RUN cd /tmp && \
    wget https://github.com/fastfloat/fast_float/archive/refs/tags/v8.0.2.tar.gz && \
    tar -xzf v8.0.2.tar.gz && \
    mkdir -p /usr/local/include && \
    cp -r fast_float-8.0.2/include/* /usr/local/include/ && \
    rm -rf /tmp/fast_float-8.0.2 /tmp/v8.0.2.tar.gz

WORKDIR /usr/src/shortest_path

# Copy build configuration and source files
COPY CMakeLists.txt ./
COPY external/ ./external/
COPY src/ ./src/
COPY test/ ./test/
COPY static/ ./static
COPY scripts/ ./scripts
COPY img_gen/ ./img_gen

# Replace <format> with <fmt/format.h> and add missing includes
RUN find . -name "*.cpp" -o -name "*.h" | xargs sed -i 's/#include <format>/#include <fmt\/format.h>/g' && \
    find . -name "*.cpp" -o -name "*.h" | xargs sed -i 's/std::format/fmt::format/g' && \
    sed -i '1i#include <climits>' src/utils/Matrix.cpp && \
    sed -i '4i#include <atomic>' src/utils/Utils.hpp

# Configure and build (this will rebuild when any C++ files change)
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_CXX_STANDARD=20 && \
    make -j$(nproc)

# Copy Python files and install dependencies
COPY requirements.txt* ./
RUN pip install --no-cache-dir flask

# Copy Python app
COPY app.py ./

EXPOSE 8000
CMD ["python3", "app.py"]