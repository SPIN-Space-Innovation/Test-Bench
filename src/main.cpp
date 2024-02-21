/**
 * MIT License
 *
 * Copyright (c) 2023 SPIN - Space Innovation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdbool.h>

#include <pico/stdlib.h>

#include "main.h"
#include "datalogger.h"

#define DATA_LOGGER_UART 1
#define DATA_LOGGER_BAUDRATE 115200
#define DATA_LOGGER_TX_PIN 11
#define DATA_LOGGER_RX_PIN 12
#define DATA_LOGGER_SAVE_PIN 10

int main() {

    stdio_init_all();

    DataLogger *logger = DataLogger::getUART(DATA_LOGGER_UART, DATA_LOGGER_BAUDRATE, DATA_LOGGER_TX_PIN, DATA_LOGGER_RX_PIN, DATA_LOGGER_SAVE_PIN);

    if (logger == nullptr) while(true) asm("");

    int i = 1;

    while (true) {

        if (logger->sendData("Hello\n", 6)){

            if (i++ == 500) {

                logger->save();
                i = 0;
            }
        }
        else fprintf(stderr, "Failed to print to data logger.\n");
    }
}