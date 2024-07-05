# Use the official Ubuntu as a parent image
FROM --platform=linux/amd64 ubuntu:20.04 AS build
# Set environment variables to avoid user interaction during installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install dependencies
RUN apt-get update && \
    apt-get install -y cmake gcc-arm-none-eabi build-essential git wget python3 python3-pip libnewlib-arm-none-eabi

# Set the working directory
WORKDIR /opt

# Install CMake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6-Linux-x86_64.sh && \
    chmod +x cmake-3.19.6-Linux-x86_64.sh && \
    ./cmake-3.19.6-Linux-x86_64.sh --skip-license --prefix=/usr/local

RUN git clone -b main https://@github.com/mdkul22/picomotion.git
ENV PICO_SDK_PATH=/opt/picomotion/extern/pico-sdk
# Export environment variables
RUN echo "export PICO_SDK_PATH=/opt/picomotion/extern/pico-sdk" >> ~/.bashrc

# Set the working directory to the example project
WORKDIR /opt/picomotion
RUN git submodule init && git submodule update --recursive
# Create a build directory
RUN mkdir build 
RUN cd build && cmake ..
RUN cd build && make -j$(nproc) 

FROM ubuntu:20.04

# Copy built files from the build stage
COPY --from=build /opt/picomotion/build /opt/picomotion/build

# Set the working directory
WORKDIR /opt/picomotion

# Default command to keep the container running
CMD ["/bin/bash"]
