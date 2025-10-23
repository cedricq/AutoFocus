FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    libopencv-dev \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy project files
COPY . .

# Create build directory
RUN mkdir build && cd build && cmake .. && make

# Define default command
CMD ["ctest"]
