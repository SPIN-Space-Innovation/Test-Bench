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

DataLogger::DataLogger(const int baudrate, const uint8_t txPin, const uint8_t rxPin, const uint8_t savePin) : baudrate{baudrate}, savePin{savePin} {
    
    // Check if the given UART is invalid or already used
    uart_inst_t *tempUart = getUART(txPin, rxPin);
    
    if (tempUart == nullptr) {
        fprintf(stderr, "Invalid UART provided.\n");
        return;
    }

    if (uart_is_enabled(tempUart)) {
        fprintf(stderr, "UART%d is already used.\n", uart_get_index(tempUart));
        return;
    }

    // Else proceed with initialization
    uart = tempUart;

    uart_init(uart, baudrate);
    
    // Set the tx and rx pins to UART mode
    gpio_set_function(txPin, GPIO_FUNC_UART);
    gpio_set_function(rxPin, GPIO_FUNC_UART);

    // Hardware flow control, Request To Send (RTS) and Clear To Send (CTS), not used
    uart_set_hw_flow(uart, false, false);

    // Set the UART data format to 8 data bits, 1 stop bit and no parity
    uart_set_format(uart, 8, 1, UART_PARITY_NONE);

    // Enable FIFO on UART
    uart_set_fifo_enabled(uart, true);

    // To avoid data loss, delay a period of time for the module to start
    sleep_ms(startupDelay);

    // Set save pin to high for future resets
    gpio_init(savePin);
    gpio_set_dir(savePin, GPIO_OUT);
    gpio_put(savePin, 1);

    printf("Initialized data logger\n");
}

DataLogger::~DataLogger() {

    save();
    
    uart_deinit(uart);

    printf("DeInitialized data logger\n");
}

bool DataLogger::sendData(const char *src, const size_t len) {

    // Data can't be sent if UART isn't enabled
    if (!uart_is_enabled(uart)) return false;

    // If FIFO has no space, wait for it to drain
    if (!uart_is_writable(uart)) {

        fprintf(stderr, "Not writtable.\n");
        uart_tx_wait_blocking(uart);
    }
    
    // Write data to UART FIFO
    uart_write_blocking(uart, reinterpret_cast<const uint8_t*>(src), len);
    
    printf("Sent data.\n");

    return true;
}

void DataLogger::save() {
    
    // Reset the save pin to save all the data to
    // the current file and proceed to the next one
    gpio_put(savePin, 0);
    sleep_ms(resetDelay);
    gpio_put(savePin, 1);

    printf("Saved data.\n");
}

// Check if the tx and rx pins correspond to UART0 or UART1 peripheral or no peripheral at all
uart_inst_t *DataLogger::getUART(const uint8_t txPin, const uint8_t rxPin) {
    
    // Available pins for UART0 peripheral
    if ((txPin == 0 || txPin == 12 || txPin == 16) && (rxPin == 1 || rxPin == 13 || rxPin == 17))
        return uart0;

    // Available pins for UART1 peripheral
    if ((txPin == 4 || txPin == 8) && (rxPin == 5 || rxPin == 9))
        return uart1;
    
    // Not a vaild UART peripheral based on the pins given
    return nullptr;
}