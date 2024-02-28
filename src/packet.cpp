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
#include <pico/time.h>

#include <hardware/adc.h>

// TODO: Include common.h in packet.cpp implementation file instead of packet.h header file without it throwing errors about the hx711_t type (for some reason uart_inst_t works for the datalogger)

#include "packet.h"

// TODO: Implement copy constructor, move constructor, destructor and assignment operators
// TODO: Check if the pins are already used

// Constructor performs setups and initializations necessary
Packet::Packet(const uint8_t pressureTransmitterPin, const uint8_t hx711dataPin, const uint8_t hx711clockPin) {

    // ADC initialization for pressure transmitter
    adc_init();
    adc_gpio_init(pressureTransmitterPin);

    // HX711 initialization:

    // Set configurations
    hx711_config_t hxcfg;
    hx711_get_default_config(&hxcfg);

    // Set data and clock pins
    hxcfg.data_pin = hx711dataPin;
    hxcfg.clock_pin = hx711clockPin;

    // Initialize
    hx711_init(&hx, &hxcfg);

    // Power up the hx711 and set gain on chip
    hx711_power_up(&hx, hx711_gain_128);

    // Wait for readings to settle
    hx711_wait_settle(hx711_rate_80);
}

// Refresh the packet, giving it new data
void Packet::getData() {
    
    timestamp = time_us_64();
    pressureTransmitterOutput = adc_read();
    loadcellOutput = hx711_get_value(&hx);
}

// Produce a string in csv format of the packet's data members and return the length of the string
int Packet::stringify(char *result) {

    return sprintf(result, "%llu,%d,%d\n", timestamp, pressureTransmitterOutput, loadcellOutput);
}