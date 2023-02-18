cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/home/djfm/code/pico-sdk")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(picolight)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(pico
    main.cpp
)

target_link_libraries(pico pico_stdlib hardware_adc)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico 1)
pico_enable_stdio_uart(pico 0)

pico_add_extra_outputs(pico)