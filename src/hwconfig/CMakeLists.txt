add_library(hwconfig hw_configure.c interrupts.c)

target_include_directories(hwconfig PUBLIC 
${CMAKE_SOURCE_DIR}/include/hwconfig)

target_link_libraries(hwconfig qmi8658 pico_stdlib)
