#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>

#ifdef __arm__
#include <bsp/bsp.h>
#endif

#define IN_MODE (GPIO_DIR_IN | GPIO_EVENT_RISE_EDGE | GPIO_EVENT_FALL_EDGE)
#define DELAY_S 1
#define GPIO_PIN_IN 9
#define GPIO_PIN_OUT 12

typedef struct CallBackContext {
    int lastPinValue;
} CallBackContext;

static void EventCallBack(
        GpioHandle handle, rtl_uint32_t pin, rtl_uint32_t pinval, void *ctx)
{
    fprintf(stderr, "GPIO%u input pin is %u\n", pin, pinval);
    ((CallBackContext *)ctx)->lastPinValue = pinval;
}

int main(int argc, const char *argv[])
{
    CallBackContext ctx = {-1};

    fprintf(stderr, "Start GPIO_echo test\n");

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

    if (GpioInit())
    {
        fprintf(stderr, "GpioInit failed\n");
        return EXIT_FAILURE;
    }

    /* initialize and setup gpio0 port */
    GpioHandle handle = NULL;
    if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE)
    {
        fprintf(stderr, "GpioOpenPort failed\n");
        return EXIT_FAILURE;
    }

    if (GpioSetMode(handle, GPIO_PIN_OUT, GPIO_DIR_OUT))
    {
        fprintf(stderr, "GpioSetMode for GPIO%d failed\n", GPIO_PIN_OUT);
        return EXIT_FAILURE;
    }

    if (GpioSetMode(handle, GPIO_PIN_IN, IN_MODE))
    {
        fprintf(stderr, "GpioSetMode for GPIO%d failed\n", GPIO_PIN_IN);
        return EXIT_FAILURE;
    }

    if (GpioRegisterEventCb(
            handle, GPIO_PIN_IN, (GpioEventCb) EventCallBack, (void*) &ctx))
    {
        fprintf(
            stderr,
            "GpioRegisterEventCb for GPIO%u failed!\n",
            GPIO_PIN_IN);

        return EXIT_FAILURE;
    }

    fprintf(stderr, "GPIO%d output pin set 1\n", GPIO_PIN_OUT);

    if (GpioOut(handle, GPIO_PIN_OUT, 1))
    {
        fprintf(stderr, "GpioOut 1 for GPIO%d failed\n", GPIO_PIN_OUT);
        return EXIT_FAILURE;
    }

    sleep(DELAY_S);

    if(ctx.lastPinValue != 1)
    {
        fprintf(stderr, "Error: wrong last input value!\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "GPIO%d output pin set 0\n", GPIO_PIN_OUT);

    if (GpioOut(handle, GPIO_PIN_OUT, 0))
    {
        fprintf(stderr, "GpioOut 0 for GPIO%d failed\n", GPIO_PIN_OUT);
        return EXIT_FAILURE;
    }

    sleep(DELAY_S);

    if(ctx.lastPinValue != 0)
    {
        fprintf(stderr, "Error: wrong last input value!\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Test successfully finished.\n");

    return EXIT_SUCCESS;
}