cmake_minimum_required(VERSION 3.14)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(capture-frame-uart LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_subdirectory(../.. pi-pico-ov767X)

file(GLOB SRCS ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

message("${PROJECT_NAME}: ${SRCS}")

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    ${SRCS}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_stdlib
    pi-pico-ov767X
    hardware_gpio
    hardware_i2c
    hardware_uart
    hardware_dma
    hardware_irq
    hardware_pio
    hardware_timer
)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})
