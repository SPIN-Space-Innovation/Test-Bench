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

#include <hardware/uart.h>

#include "datalogger.h"

DataLogger::DataLogger(uart_t uart, uint8_t txPin, uint8_t rxPin, uint8_t savePin) {

    this->uart = uart;
    this->txPin = txPin;
    this->rxPin = rxPin;
    this->savePin = savePin;

    printf("Initialized UART %u for communication with data logger\n", uart_get_index(uart));

    gpio_set_function(txPin, GPIO_FUNC_UART);
    gpio_set_function(rxPin, GPIO_FUNC_UART);

    uart_set_hw_flow(uart, false, false);
    uart_set_format(uart, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(uart, true);

    sleep_ms(startupDelay);

    gpio_init(savePin);
    gpio_set_dir(savePin, GPIO_OUT);
    gpio_put(savePin, 1);
}

DataLogger* DataLogger::getUART(uint8_t uartIDNum, int baudrate, uint8_t txPin, uint8_t rxPin, uint8_t savePin) {

    if (uartIDNum > 1) {
        fprintf(stderr, "UART %hhu does not exist\n", uartIDNum);
        return nullptr;
    }

    uart_t uart = (uartIDNum) ? uart1 : uart0;

    if (!uart_is_enabled(uart))
        uart_init(uart, baudrate);

    if (!uart_is_enabled(uart)) {
        fprintf(stderr, "Failed to initialize UART %hhu\n", uartIDNum);
        return nullptr;
    }

    return new DataLogger(uart, txPin, rxPin, savePin);
}

bool DataLogger::sendData(const char *src, size_t len) {

    if (!uart_is_enabled(uart)) return false;

    if (!uart_is_writable(uart))
        uart_tx_wait_blocking(uart);

    uart_write_blocking(uart, (uint8_t*)src, len);

    return true;
}

void DataLogger::save() {
    
    gpio_put(savePin, 0);
    sleep_ms(resetDelay);
    gpio_put(savePin, 1);
}

DataLogger::~DataLogger() {

    save();
    
    printf("DeInitialized UART %u for communication with data logger\n", uart_get_index(uart));

    uart_deinit(uart);
}