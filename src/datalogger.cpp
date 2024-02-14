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
#include <stdint.h>

#include <pico/stdlib.h>

#include <hardware/uart.h>

#define SAVE_PIN 7

#define STARTUP_DELAY 2000
#define RESET_DELAY 500

bool setupDataLogger(uint8_t uartIDNum) {

    if (uartIDNum > 1) return false;

    auto uart = (uartIDNum) ? uart1 : uart0;

    if (!uart_is_enabled(uart))
        uart_init(uart, 115200);
    
    if (!uart_is_enabled(uart)) return false;

    uart_set_fifo_enabled(uart, true);
    uart_set_format(uart, 8, 1, UART_PARITY_NONE);
    uart_set_hw_flow(uart, false, false);

    sleep_ms(STARTUP_DELAY);
    gpio_init(SAVE_PIN);
    gpio_set_dir(SAVE_PIN, GPIO_OUT);
    gpio_put(SAVE_PIN, 1);

    return true;
}

bool printDataLogger(uint8_t uartIDNum, const char *src, size_t len) {

    if (uartIDNum > 1) return false;

    auto uart = (uartIDNum) ? uart1 : uart0;

    if (!uart_is_enabled(uart)) return false;

    if (!uart_is_writable(uart))
        uart_tx_wait_blocking(uart);

    uart_write_blocking(uart, (uint8_t*)src, len);

    return true;
}

void nextFile(uint8_t uartIDNum) {

    if (uartIDNum > 1) return;

    auto uart = (uartIDNum) ? uart1 : uart0;

    gpio_put(SAVE_PIN, 0);
    sleep_ms(RESET_DELAY);
    gpio_put(SAVE_PIN, 1);
    sleep_ms(RESET_DELAY / 2);
}