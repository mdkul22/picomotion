#!/bin/bash

run_builder() {
 DEBUG=false
 FLASH=false
 CLEAN=false
 DEBUGR=false
 HALT=false
while [[ $# -gt 0 ]]; do
  case $1 in
    -d|--debug)
      DEBUG=true
      shift
      ;;
    -f|--flash)
      FLASH=true
      shift
      ;;
    -c|--clean)
      CLEAN=true
      shift
      ;;
    -H|--halt)
      HALT=true
      shift
      ;;
    -o|--openocd)
      DEBUGR=true
      shift
      ;;
    -h|--help)
      echo "      -d|--debug to enable debug flag for build
      -f|--flash to flash after build 
      -c|--clean to clean before build"
      exit 1
      shift
      ;;
    *)
      # unknown option
      ;;
  esac
done
if [[ $CLEAN == true ]]; then
 echo "removing build folder"
 rm -rf build
fi
build_binary "DEBUG"
if [[ $FLASH == true ]]; then
echo "flashing"
flash_binary
fi
if [[ $HALT == true ]]; then
echo "halting"
flash_halt_binary
fi
if [[ $DEBUGR == true ]]; then
echo "entering openocd"
debug_binary
fi

}


build_binary() {
# Create a build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

export PICO_SDK_PATH="/Users/mdk/SDKs/pico-sdk"
# Navigate to the build directory
cd build
# Run CMake to configure the project
if [[ $1 == "DEBUG" ]]; then
 echo "enabling debug flag"
 cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
else 
 cmake ..
fi
# Build the project
cmake --build . --parallel 
cp compile_commands.json ..
}

flash_binary() {
# should be in build directory
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program picomotion.elf verify reset exit"
}

flash_halt_binary() {
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program picomotion.elf verify reset halt exit"
}

debug_binary() {
# should be in build directory
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg 
}

run_builder $@

