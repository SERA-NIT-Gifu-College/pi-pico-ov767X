cmake_minimum_required(VERSION 3.14)

project(pi-pico-ov767X LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRCS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_sources(${PROJECT_NAME} INTERFACE ${SRCS})
message("${PROJECT_NAME}: ${SRCS}")

target_link_libraries(${PROJECT_NAME} INTERFACE
    pico_stdlib
    hardware_clocks
    hardware_dma
    hardware_gpio
    hardware_i2c
    hardware_irq
    hardware_pio
    hardware_pwm
)

pico_generate_pio_header(${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/camera-io.pio
)
