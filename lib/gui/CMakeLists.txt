add_library(gui gui.c)
target_include_directories(gui PUBLIC ${CMAKE_SOURCE_DIR}/lib/gui ${CMAKE_SOURCE_DIR}/extern/lvgl)
target_link_libraries(gui PUBLIC gc9a01 lvgl)
