# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/Users/david/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(lcd C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


set(SPI_TFT_PORT "spi0" CACHE STRING "TFT spi port-name")
set(SPI_TFT_CS   "9"   CACHE STRING "TFT CS  pin number")
set(SPI_TFT_DC   "10"   CACHE STRING "TFT DC  pin number")
set(SPI_TFT_RST  "11"   CACHE STRING "TFT RST pin number")
set(TFT_OPTIONS TFT_ENABLE_BLACK TFT_ENABLE_RESET TFT_ENABLE_TEXT TFT_ENABLE_SHAPES
        TFT_ENABLE_ROTATE TFT_ENABLE_SCROLL
        CACHE STRING "TFT options/functions")

# Add executable. Default name is the project name, version 0.1

add_compile_definitions(SPI_TFT_PORT=${SPI_TFT_PORT})
add_compile_definitions(SPI_TFT_CS=${SPI_TFT_CS})
add_compile_definitions(SPI_TFT_DC=${SPI_TFT_DC})
add_compile_definitions(SPI_TFT_RST=${SPI_TFT_RST})

foreach(opt IN LISTS TFT_OPTIONS)
    message(">>> using: ${opt}")
    add_compile_definitions(${opt})
endforeach(opt)


add_subdirectory(libs/lfs)
add_subdirectory(libs/lcd)
add_subdirectory(libs/microbuffer)

#target_link_libraries(lcd PUBLIC )



add_executable(lcd lcd.cpp s_lib.cpp s_lib.h bmp.h bmp.cpp)

#target_include_directories(lcd PUBLIC
#        "${PROJECT_BINARY_DIR}"
#        "${PROJECT_SOURCE_DIR}/libs/lcd"
#        )

pico_set_program_name(lcd "lcd")
pico_set_program_version(lcd "0.1")

pico_enable_stdio_uart(lcd 1)
pico_enable_stdio_usb(lcd 0)

# Add the standard library to the build
target_link_libraries(lcd pico_stdlib)

# Add any user requested libraries
target_link_libraries(lcd
        hardware_spi
        hardware_flash
        st7735
        lfs
        microbuffer
        )

pico_add_extra_outputs(lcd)

