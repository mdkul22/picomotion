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
set(LV_CONF_PATH "${CMAKE_SOURCE_DIR}/lib/gui/lv_conf.h")

add_subdirectory(lib/hwconfig)
add_subdirectory(lib/gc9a01)
add_subdirectory(lib/qmi8658)
add_subdirectory(extern/lvgl)
add_subdirectory(lib/gui)

# lvgl stuff
include_directories(
  ${CMAKE_SOURCE_DIR}/lib/gui
  extern/lvgl
  extern/lvgl/src
)

message(STATUS "Including lv_conf.h from ${LV_CONF_PATH} If there are compilation errors related to lv_conf.h, please check that this file is correctly configured according to your needs and that it is in the correct directory.")

add_executable(picomotion
    app/picomotion.c
    lib/hwconfig/sys_common.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(picomotion
    pico_stdlib
    hardware_i2c
    hwconfig
    qmi8658
    lvgl
    gc9a01
    gui
)

#  enable both usb and main
pico_enable_stdio_usb(picomotion 1)
pico_enable_stdio_uart(picomotion 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(picomotion)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(picomotion)

