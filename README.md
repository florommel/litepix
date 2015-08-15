# Litepix

## General Information

Litepix is a lightweight software application to run a led pixel matrix build
with WS2811 RGB (or similar) controllers.  It can currently be compiled for
Atmel AVR8 microcontrollers but could probably be ported to other architectures.

The communication with the WS2811 controllers and the whole processing
(including animations and small games or other applications) is done on a single
microcontroller.  Currently this is an ATmega clocked with 16Mhz and at least
2048 bytes of RAM.  User input (through several devices like ir remotes or
bluetooth (not implemented yet)) is captured by another controller and sent to
the main controller via USART.

The implementation for the main controller is located in `main`.
The implementation for the input controller is located in `input`.

There is a simulator integrated which allows to test the program on a computer
without any additional hardware required (gtk3 is needed).


## Compile and program

*   Compile and run simulation

        $ cd <repository_root>/main
        $ make ARCH=sim
        $ ./build-sim/litepix.elf

*   Compile and program main controller (You probably have to change the values
    of `MCU` and `AVRDUDE_FLAGS` in `arch-avr.mk` according to your needs)

        $ cd <repository_root>/main
        $ make program

*   Compile and program input controller (You probably have to change the values
    of `MCU` and `AVRDUDE_FLAGS` in `arch-avr.mk` according to your needs)

        $ cd <repository_root>/input
        $ make program
