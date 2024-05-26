# Use the official Ubuntu as a parent image
FROM --platform=linux/amd64 ubuntu:20.04 AS build
# Set environment variables to avoid user interaction during installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install dependencies
RUN apt-get update && \
    apt-get install -y cmake gcc-arm-none-eabi build-essential git wget python3 python3-pip libnewlib-arm-none-eabi

# Set the working directory
WORKDIR /opt

# Clone the Pico SDK
RUN git clone -b master https://github.com/raspberrypi/pico-sdk.git

# Set up environment variables for the Pico SDK
ENV PICO_SDK_PATH=/opt/pico-sdk

# Export environment variables
RUN echo "export PICO_SDK_PATH=/opt/pico-sdk" >> ~/.bashrc

# Install CMake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6-Linux-x86_64.sh && \
    chmod +x cmake-3.19.6-Linux-x86_64.sh && \
    ./cmake-3.19.6-Linux-x86_64.sh --skip-license --prefix=/usr/local

RUN git clone -b main https://@github.com/mdkul22/picomotion.git

# Set the working directory to the example project
WORKDIR /opt/picomotion

# Create a build directory
RUN mkdir build

# Set the working directory to the build directory
WORKDIR /opt/picomotion/build

# Configure the project
RUN cmake ..

# Build the project
RUN make -j$(nproc)

FROM ubuntu:20.04

# Copy built files from the build stage
COPY --from=build /opt/picomotion/build /opt/picomotion/build

# Set the working directory
WORKDIR /opt/picomotion

# Default command to keep the container running
CMD ["/bin/bash"]
