cmake_minimum_required(VERSION 3.17)

if(NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "Set PICO_SDK_PATH to the Raspberry Pi Pico SDK checkout")
endif()
include("$ENV{PICO_SDK_PATH}/pico_sdk_init.cmake")

project(pico_matrix_keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
pico_sdk_init()

add_library(pico_matrix_keypad src/keypad.c)
target_include_directories(pico_matrix_keypad PUBLIC include)
target_link_libraries(pico_matrix_keypad PUBLIC pico_stdlib)

add_executable(key_terminal examples/key-terminal/main.c)
target_link_libraries(key_terminal PRIVATE pico_matrix_keypad)
pico_enable_stdio_uart(key_terminal 0)
pico_enable_stdio_usb(key_terminal 1)
pico_add_extra_outputs(key_terminal)
