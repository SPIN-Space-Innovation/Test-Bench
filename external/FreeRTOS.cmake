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

Include(FetchContent)

FetchContent_Declare(
    FreeRTOS
    GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel
    GIT_TAG V11.0.1
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_Populate(FreeRTOS)

FILE(GLOB FreeRTOS_SRC ${freertos_SOURCE_DIR}/*.c)

add_library(FreeRTOS STATIC
    ${FreeRTOS_SRC}
    ${freertos_SOURCE_DIR}/portable/GCC/ARM_CM0/port.c
    ${freertos_SOURCE_DIR}/portable/MemMang/heap_4.c
)

target_include_directories(FreeRTOS PUBLIC
    ${freertos_SOURCE_DIR}/include
    include/
    ${freertos_SOURCE_DIR}/portable/GCC/ARM_CM0
)
