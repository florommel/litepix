# litepix

## General Information

litepix is a lightweight software application to run a led pixel matrix build with
WS2811 RGB (or similar) controllers.  It can currently be compiled for Atmel AVR8
microcontrollers but could probably be ported to other architectures.

The communication with the WS2811 controllers and the whole processing
(including animations and small games or other applications) is  done on a single
microcontroller.  Currently this is an ATmega clocked with 16Mhz and at least
2048 bytes of RAM.  User Input (through several devices like ir remotes or bluetooth)
are captured by another controller and sent to the main controller via usart
(this is not implemented yet).

The implementation for the main controller is located in `main`

There is a simulator integrated which allows you to test the program on your
computer without any additional hardware required (gtk3 is needed).


## Compile and program

*   cd to a source directory (currently only `main`)

*   Compile and run simulation

        $ make ARCH=sim
        $ ./build-sim/litepix.elf

*   Compile and program the controller (You probably have to change the values
    of `MCU` and `AVRDUDE_FLAGS` in `arch-avr.mk` according to your needs)

        $ make program
