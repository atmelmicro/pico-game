cmake_minimum_required(VERSION 3.13)

include(${PICO_SDK_INIT_CMAKE_FILE})

project(microbuffer)
add_library(${PROJECT_NAME} microbuffer.cpp microbuffer.h)
target_include_directories(${PROJECT_NAME} PUBLIC .)