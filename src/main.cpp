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

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include <pico/stdlib.h>

#include "common.h"

#include "main.h"
#include "datalogger.h"

#define DATA_LOGGER_BAUDRATE 115200
#define DATA_LOGGER_TX_PIN 8
#define DATA_LOGGER_RX_PIN 9
#define DATA_LOGGER_SAVE_PIN 7

#define HX711_CLK_PIN 14
#define HX711_DATA_PIN 15

#define BUFFER_SIZE 256

int main() {

    stdio_init_all();

    // Initialize HX711
    hx711_config_t hxcfg;
    hx711_get_default_config(&hxcfg);
    hxcfg.clock_pin = HX711_CLK_PIN;
    hxcfg.data_pin = HX711_DATA_PIN;

    hx711_t hx;
    hx711_init(&hx, &hxcfg);
    hx711_power_up(&hx, hx711_gain_128);
    hx711_wait_settle(hx711_rate_80);

    // Create logger instance
    DataLogger logger(DATA_LOGGER_BAUDRATE, DATA_LOGGER_TX_PIN, DATA_LOGGER_RX_PIN, DATA_LOGGER_SAVE_PIN);

    logger.sendData("HX711\n", 6);

    while (true) {
        int32_t hx711Reading = hx711_get_value(&hx);

        char buffer[BUFFER_SIZE];

        int outputSize = snprintf(buffer, BUFFER_SIZE, "%" PRId32 "\n", hx711Reading);

        logger.sendData(buffer, outputSize);
    }
}