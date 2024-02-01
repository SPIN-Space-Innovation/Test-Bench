# MIT License
#
# Copyright (c) 2024 SPIN - Space Innovation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.19)

include(${CMAKE_CURRENT_LIST_DIR}/external/pico-sdk.cmake)

# initialize the SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)

project(template)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# initialize the Pico SDK
pico_sdk_init()

include("${CMAKE_CURRENT_LIST_DIR}/src/CMakeLists.txt")

add_executable(template ${${PROJECT_NAME}_SRC})

include(${CMAKE_CURRENT_LIST_DIR}/external/FreeRTOS.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/external/etl.cmake)

target_link_libraries(template
    hardware_adc 
    hardware_base
    hardware_claim
    hardware_clocks
    hardware_divider
    hardware_dma
    hardware_exception
    hardware_flash
    hardware_gpio
    hardware_i2c
    hardware_interp
    hardware_irq
    hardware_gpio
    hardware_pll
    hardware_pwm
    hardware_resets
    hardware_rtc
    hardware_spi
    hardware_sync
    hardware_timer
    hardware_uart
    hardware_vreg
    hardware_watchdog
    hardware_xosc
    pico_i2c_slave
    pico_multicore
    pico_rand
    pico_stdlib
    pico_sync
    pico_time
    pico_unique_id
    pico_util
    pico_base
    pico_binary_info
    pico_bit_ops
    pico_bootrom
    pico_bootsel_via_double_reset
    pico_cxx_options
    pico_divider
    pico_double
    pico_float
    pico_int64_ops
    pico_malloc
    pico_mem_ops
    pico_platform
    pico_printf
    pico_runtime
    pico_stdio
    etl::etl
    FreeRTOS)

pico_add_extra_outputs(template)

pico_enable_stdio_usb(template 0)
pico_enable_stdio_uart(template 1)
