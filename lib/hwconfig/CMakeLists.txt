add_library(hwconfig hw_configure.c interrupts.c)

target_include_directories(hwconfig PUBLIC 
${CMAKE_SOURCE_DIR}/lib/hwconfig)

target_link_libraries(hwconfig qmi8658 pico_stdlib hardware_spi hardware_i2c hardware_pwm hardware_adc hardware_dma)
