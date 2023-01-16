cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/home/djfm/code/pico-sdk")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(pico
    main.cpp
)

target_link_libraries(pico pico_stdlib)
pico_add_extra_outputs(pico)