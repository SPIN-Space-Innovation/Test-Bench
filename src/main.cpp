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

#include <hardware/adc.h>
#include <common.h>

#include "main.h"
#include "datalogger.h"

#define DATA_LOGGER_BAUDRATE 115200
#define DATA_LOGGER_TX_PIN 8
#define DATA_LOGGER_RX_PIN 9
#define DATA_LOGGER_SAVE_PIN 7

#define BUFFER_SIZE 100000
#define SAVE_REQUIREMENT 10

#define PRESSURE_TRANSMITTER_PIN 26

#define HX711_DATA_PIN 0
#define HX711_CLOCK_PIN 1

// TODO: The output to the data logger appears to have some very minor data losses (2 characters in a million or something and some characters on the last line of each file)

int main() {

    stdio_init_all();

    // Create logger instance
    DataLogger logger(DATA_LOGGER_BAUDRATE, DATA_LOGGER_TX_PIN, DATA_LOGGER_RX_PIN, DATA_LOGGER_SAVE_PIN);

    // Buffer for temporary data storage before sending to logger
    char *buffer = new char[BUFFER_SIZE];
    int bufferCounter = 0;

    // Counter for saving grouped data
    int saveCounter = 0;

    // ADC initialization for pressure transmitter
    adc_init();
    adc_gpio_init(PRESSURE_TRANSMITTER_PIN);

    // HX711 initialization:
    hx711_t hx;

    // Set configurations
    hx711_config_t hxcfg;
    hx711_get_default_config(&hxcfg);

    // Set data and clock pins
    hxcfg.data_pin = HX711_DATA_PIN;
    hxcfg.clock_pin = HX711_CLOCK_PIN;

    // Initialize
    hx711_init(&hx, &hxcfg);

    // Power up the hx711 and set gain on chip
    hx711_power_up(&hx, hx711_gain_128);

    // Wait for readings to settle
    hx711_wait_settle(hx711_rate_80);

    while (true) {

        // Create a Packet instance
        uint64_t timestamp = time_us_64();
        int pressureTransmitterOutput = adc_read();
        int loadcellOutput = hx711_get_value(&hx);

        // Get the string format of the packet
        char packetString[100];
        int len = snprintf(packetString, 100, "%llu,%d,%d\n", timestamp, pressureTransmitterOutput, loadcellOutput);

        // If data is not written to the string, terminate program execution
        if (packetString == nullptr) asm("");

        // Check if the string can fit the buffer
        if (bufferCounter + len > BUFFER_SIZE) {
            
            // Send the data to the UART FIFO
            if (logger.sendData(buffer, bufferCounter)) {
                
                // Every 10 times the buffer is sent, all the data will be saved to the corresponding file
                if (++saveCounter == SAVE_REQUIREMENT) {
                    logger.save();
                    saveCounter = 0;
                }
            }

            // Empty the buffer
            bufferCounter = 0;
        }

        // Add the string to the buffer
        bufferCounter += sprintf(buffer + bufferCounter, packetString);
    }
}