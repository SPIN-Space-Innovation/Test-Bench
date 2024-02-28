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

#pragma once

class DataLogger {

private:

    // Time to wait after startup to avoid data loss
    const int startupDelay = 2000;

    // Time to wait during reset
    const int resetDelay = 500;

    // UART peripheral that is used by the datalogger
    uart_inst_t *uart;

    // Baudrate for uart communication with the datalogger
    int baudrate;

    // Pins assigned for the datalogger
    uint8_t savePin;

public:
    // Constructor
    DataLogger(const int baudrate, const uint8_t txPin, const uint8_t rxPin, const uint8_t savePin);

    // Destructor
    ~DataLogger();

    // Deletion (at least temporarily) of the rest of the constructors
    DataLogger(const DataLogger&) = delete;
    DataLogger(DataLogger&&) noexcept = delete;
    DataLogger& operator=(const DataLogger&) = delete;
    DataLogger& operator= (DataLogger&&) noexcept = delete;

    // Send data to data logger
    bool sendData(const char *src, const size_t len);
    
    // Reset the save pin to save the data and switch file
    void save();

    // Get UART corresponding to the given tx and rx pins
    static uart_inst_t *getUART(const uint8_t txPin, const uint8_t rxPin);
};