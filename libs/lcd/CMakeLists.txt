cmake_minimum_required(VERSION 3.13)

include(${PICO_SDK_INIT_CMAKE_FILE})

project(st7735)
add_library(${PROJECT_NAME} ST7735_TFT.cpp hw.cpp)
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi)
target_include_directories(${PROJECT_NAME} PUBLIC .)