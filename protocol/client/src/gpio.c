//реализация процесса, которая получает ШИМ-сигнал c ардуино

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>

#ifdef __arm__
#include <bsp/bsp.h>
#endif

//пины для реализации собственного протокола
#define PROTOCOL_RX_PIN 3
#define PROTOCOL_TX_PIN 4
#define PROTOCOL_CONNECT 5

int main(int argc, const char *argv[])
{
    bool conn = false;     //переменные для подключения к другому устройству
    uint8_t pwmSign = 0;   //переменная для хранения полученного ШИМ-сигнала
    bool rx, tx = false;
    fprintf(stderr, "Start receiving pwm-signal test\n");

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

    //инициализация gpio
    if (GpioInit()) {
        fprintf(stderr, "GpioInit failed\n");
        return EXIT_FAILURE;
    }

    //инициализация gpio0 порта
    GpioHandle handle = NULL;
    if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE) {
        fprintf(stderr, "GpioOpenPort failed\n");
        return EXIT_FAILURE;
    }

    //настройка портов для протокола
    if (GpioSetMode(handle, PROTOCOL_RX_PIN, GPIO_DIR_IN) || GpioSetMode(handle, PROTOCOL_TX_PIN, GPIO_DIR_OUT) || GpioSetMode(handle, PROTOCOL_CONNECT, GPIO_DIR_IN)) {
        fprintf(stderr, "GpioSetMode for protocol failed\n");
        return EXIT_FAILURE;
    }

    //проверка подключения c устройством
    while (rx == false) {
        if(GpioIn(handle, PROTOCOL_RX_PIN, &rx)) {
        conn = true;
        GpioOut(handle, PROTOCOL_TX_PIN, conn);
        }
    }
    tx = false;
    GpioOut(handle, PROTOCOL_TX_PIN, tx);
    fprintf (stderr, "Devices are connected\n");

    //бесконечный цикл по получению ШИМ-сигнала и выводе его на экран по UART-шине на экран 
    while(conn = true) {
        pwmSign = 0;
        for (int i = 0; i <= 7; i++) {
            GpioIn (handle, PROTOCOL_RX_PIN, &rx);
            pwmSign = (pwmSign | rx) << 1;
            tx = rx;
            GpioOut (handle, PROTOCOL_TX_PIN, tx);
        }
        fprintf (stderr, "PWM sign: %d\n", pwmSign);
    }

    return EXIT_SUCCESS;
}