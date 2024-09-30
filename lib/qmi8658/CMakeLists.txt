add_library(qmi8658 qmi8658.c)
target_include_directories(qmi8658 PUBLIC 
${CMAKE_SOURCE_DIR}/lib/qmi8658 
${CMAKE_SOURCE_DIR}/lib/common
)
target_link_libraries(qmi8658 hwconfig hardware_i2c hardware_gpio)
