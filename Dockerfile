FROM ubuntu:22.04

# Avoid tzdata interactive prompt
ENV DEBIAN_FRONTEND=noninteractive

# Set timezone to UTC and install dependencies
RUN apt-get update && \
    apt-get install -y tzdata && \
    ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata && \
    apt-get install -y \
        build-essential \
        libopencv-dev \
        cmake \
        git \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy project files
COPY . .

# Create build directory
RUN mkdir build && cd build && cmake .. && make

WORKDIR /app/build

# Define default command
CMD ["ctest"]
