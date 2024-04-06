cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()
# rest of your project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(inc)

add_subdirectory(src)

add_executable(main
    main.c
    src/hw_init.c
    src/hw_configure.c
    src/sys_common.c
)

#  enable both usb and main
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(main)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(main 
    pico_stdlib
    qmi8658
    hardware_i2c
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(main)
