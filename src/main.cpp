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

#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include <pico/stdlib.h>

#include <FreeRTOS.h>
#include <task.h>

#include "FreeRTOSConfig.h"

void vBlinkTask(void *)
{
    while(true)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelay(250);

        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        vTaskDelay(250);
    }
}

void vHelloWorldTask(void *)
{
    while(true)
    {
        printf("Hello world\n");
        vTaskDelay(1000);
    }
}
int main()
{
    // Initialize the stdio library
    stdio_init_all();

    // Initialize the GPIO
    gpio_init(PICO_DEFAULT_LED_PIN);

    // Set the LED pin as an output device
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // Create background blink task
    BaseType_t blinkTask = xTaskCreate(vBlinkTask, "Blink Task", 128, NULL, 1, NULL);

    // Create background hello world task
    BaseType_t helloWorldTask = xTaskCreate(vHelloWorldTask, "Hello World Task", 128, NULL, 1, NULL);

    // Start background tasks
    vTaskStartScheduler();

    while (true);
}
