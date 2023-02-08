cmake_minimum_required(VERSION 3.13)

include(${PICO_SDK_INIT_CMAKE_FILE})

project(lfs)
add_library(${PROJECT_NAME} lfs.c pico_hal.c)
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_flash)
target_include_directories(${PROJECT_NAME} PUBLIC .)