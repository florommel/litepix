# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

CC = gcc
CFLAGS = -ggdb -c -std=gnu99 -Wall -Wstrict-prototypes `pkg-config --cflags --libs gtk+-3.0`
LDFLAGS = `pkg-config --cflags --libs gtk+-3.0` -Wall


ALLSUBDIRS = $(SUBDIRS) $(SUBDIRS_SIM)


TARGET = $(BUILDDIR)/$(NAME)


.PHONY: all


all: $(TARGET).elf

