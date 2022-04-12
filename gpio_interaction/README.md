### GPIO Echo Example
This example allows you to check I/O GPIO pins functionality, and also GPIO interrupts handling. The output pin (GPIO_PIN_OUT) should be connected to the input pin (GPIO_PIN_IN).
Description of the example execution:
1. At the beginning of execution, in the console will be displayed a message : "Start GPIO_echo test".
2. The BSP driver will be initialized, and GPIO pins will be configured with help by it.
3. Port "gpio0" will be opened.
4. The configuration will be set for the output pin (the pin number is defined in the GPIO_PIN_OUT macro), and for the input pin (GPIO_PIN_IN). For the last settings are specified in the IN_MODE macro.
5. An interrupt handler will be registered for the input pin.
6. The state of the output pin will be changed several times. In the case of example`s correct work: when the output pin state changes, an interrupt handler must be called and print input pin state, output and input pins state must match.
7. If the example is successful, the message “Test completed” will be displayed in the console.
If any errors occur during the execution of the example, a message about the error will be displayed in the console, the execution of the example will be stoped.
GPIO_PIN_OUT - the number of the GPIO pin configured for the output
GPIO_PIN_IN - GPIO pin number configured for input
IN_MODE - settings for input pin
DELAY_S - delay between changing the state of the output pin 
For details please refer to the [documentation](https://click.kaspersky.com/?hl=en-us&customization=kcebeta&link=online_help&pid=kos&version=1.0).

### Prerequisites
Before you begin, ensure that you have met the following requirements:
- You have installed the latest version of [KasperskyOS Community Edition](https://os.kaspersky.com/development/download/)
- You have Debian GNU/Linux "Buster" 10.7 x64

### Usage
To build the example and run the resulting image please refer to the documentation.

© 2021 AO Kaspersky Lab
