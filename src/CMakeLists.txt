add_library(qmi8658 qmi8658.c)

# Include directories specific to your library if needed
# target_include_directories(qmi8658 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../inc)
target_link_libraries(qmi8658 hardware_i2c)

