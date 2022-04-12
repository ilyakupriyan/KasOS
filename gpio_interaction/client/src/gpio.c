#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>

#ifdef __arm__
#include <bsp/bsp.h>
#endif

#define GPIO_PIN_IN 9
#define GPIO_PIN_OUT 12

int main(int argc, const char *argv[]) {
    int pinVal;
    fprintf(stderr, "Start GPIO_interaction test\n");

#ifdef __arm__
    /**
     * Initialize board support package (BSP) driver and set configuration
     * for GPIO pins. It`s required for stdout by UART.
     */
    {
        BspError rc = BspInit(NULL);
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to initialize BSP\n");
            return EXIT_FAILURE;
        }

        rc = BspSetConfig("gpio0", "raspberry_pi4b.default");
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to set mux configuration for gpio0 module\n");
            return EXIT_FAILURE;
        }
    }
#endif

    if (GpioInit()) {
        fprintf(stderr, "GpioInit failed\n");
        return EXIT_FAILURE;
    }

    /* initialize and setup gpio0 port */
    GpioHandle handle = NULL;
    if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE) {
        fprintf(stderr, "GpioOpenPort failed\n");
        return EXIT_FAILURE;
    }

    if (GpioSetMode(handle, GPIO_PIN_OUT, GPIO_DIR_OUT)) {
        fprintf(stderr, "GpioSetMode for GPIO%d failed\n", GPIO_PIN_OUT);
        return EXIT_FAILURE;
    }

    if (GpioSetMode(handle, GPIO_PIN_IN, GPIO_DIR_IN)) {
        fprintf(stderr, "GpioSetMode for GPIO%d failed\n", GPIO_PIN_IN);
        return EXIT_FAILURE;
    }

    while (true) {
        GpioIn(handle, GPIO_PIN_IN, &pinVal);
        if (pinVal == 1) {
            if (GpioOut(handle, GPIO_PIN_OUT, 1)) {
                fprintf(stderr, "GpioOut 1 for GPIO%d failed\n", GPIO_PIN_OUT);
                return EXIT_FAILURE;
            }
        } 
        if (pinVal == 0) {
            if (GpioOut(handle, GPIO_PIN_OUT, 0)) {
                fprintf(stderr, "GpioOut 1 for GPIO%d failed\n", GPIO_PIN_OUT);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}