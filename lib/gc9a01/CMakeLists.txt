add_library(gc9a01 gc9a01.c)
target_include_directories(gc9a01 PUBLIC ${CMAKE_SOURCE_DIR}/lib/gc9a01)
target_link_libraries(gc9a01 hwconfig hardware_gpio hardware_spi)
