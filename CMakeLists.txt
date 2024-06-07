cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(cmake/pico_sdk_import.cmake)

project(picomotion VERSION 1.0
                    DESCRIPTION "An improved waveshare 1.28LCD RP2040 firmware"
)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()
# rest of your project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/gc9a01)
add_subdirectory(src/qmi8658)
add_subdirectory(src/hwconfig)

add_executable(picomotion
    app/picomotion.c
    src/sys_common.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(picomotion
    pico_stdlib
    hardware_i2c
    hwconfig
    qmi8658
    # gc9a01
)

#  enable both usb and main
pico_enable_stdio_usb(picomotion 1)
pico_enable_stdio_uart(picomotion 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(picomotion)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(picomotion)

