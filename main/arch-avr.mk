# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

MCU = atmega328p
DF_CPU = 16000000
AVRDUDE_FLAGS=-c usbasp

CC = avr-g++
CFLAGS = -c -std=c++11 -mmcu=$(MCU) -D F_CPU=$(DF_CPU) -D ARCH_AVR -Os \
	     -fpack-struct -fshort-enums -Wall -Wextra $(IQUOTE)
LDFLAGS = -mmcu=$(MCU) -Wall $(IQUOTE)


ALLSUBDIRS = $(SUBDIRS) $(SUBDIRS_AVR)


TARGET = $(BUILDDIR)/$(NAME)


.PHONY: all program size


all: $(TARGET).hex


program: $(TARGET).hex
	@echo write program to flash
	avrdude -p$(MCU) $(AVRDUDE_FLAGS) -Uflash:w:$(TARGET).hex:a


size: $(TARGET).hex
	@avr-size --mcu=$(MCU) -C $(TARGET).elf


$(TARGET).hex: $(TARGET).elf
	@echo generate hex file
	@avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex
