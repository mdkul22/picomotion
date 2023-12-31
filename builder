#!/bin/bash

# Create a build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

export PICO_SDK_PATH="/Users/mdk/SDKs/pico-sdk"
# Navigate to the build directory
cd build

# Run CMake to configure the project
cmake ..

# Build the project
cmake --build .

cp compile_commands.json ..
