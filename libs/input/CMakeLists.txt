cmake_minimum_required(VERSION 3.13)

include(${PICO_SDK_INIT_CMAKE_FILE})

project(input)
add_library(${PROJECT_NAME} input.cpp)
target_link_libraries(${PROJECT_NAME} pico_stdlib)
target_include_directories(${PROJECT_NAME} PUBLIC .)