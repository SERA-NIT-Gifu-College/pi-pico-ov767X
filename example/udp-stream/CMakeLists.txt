cmake_minimum_required(VERSION 3.14)

set(PICO_BOARD pico_w)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(udp-stream LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_subdirectory(../.. pi-pico-ov767X)

file(GLOB SRCS ${CMAKE_CURRENT_LIST_DIR}/*.c)

message("${PROJECT_NAME}: ${SRCS}")

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    ${SRCS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    WIFI_SSID=\"$ENV{WIFI_SSID}\"
    WIFI_PASSWD=\"$ENV{WIFI_PASSWD}\"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_stdlib
    pi-pico-ov767X
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_dma
    hardware_i2c
    hardware_irq
    hardware_pio
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})
