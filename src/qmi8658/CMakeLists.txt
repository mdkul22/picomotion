add_library(qmi8658 qmi8658.c)
target_include_directories(qmi8658 PUBLIC 
${CMAKE_SOURCE_DIR}/include/qmi8658 
${CMAKE_SOURCE_DIR}/include/common
)
target_link_libraries(qmi8658 hardware_i2c hardware_gpio)
