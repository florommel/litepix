# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

CC = g++
CFLAGS = -ggdb -c -std=c++11 -Wall `pkg-config --cflags --libs gtk+-3.0` \
         -D ARCH_SIM $(IQUOTE)
LDFLAGS = `pkg-config --cflags --libs gtk+-3.0` -Wall $(IQUOTE)


ALLSUBDIRS = $(SUBDIRS) $(SUBDIRS_SIM)


TARGET = $(BUILDDIR)/$(NAME)


.PHONY: all


all: $(TARGET).elf

