# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

NAME = litepix

# subdirectories which contain source files
SUBDIRS = core util

# architecture-specific subdirectories
SUBDIRS_AVR = core/avr
SUBDIRS_SIM = core/sim

# default architeture (if ARCH=.. not specified on command line)
ARCH = avr

# build directories
BUILDDIR_PREFIX = build-
ALL_ARCHS = avr sim
BUILDDIR := $(BUILDDIR_PREFIX)$(ARCH)
ALL_BUILDDIRS := $(foreach dir,$(ALL_ARCHS),$(BUILDDIR_PREFIX)$(dir))

# include architecture specific mk-files
ifeq ($(ARCH),avr)
include arch-avr.mk
else ifeq ($(ARCH),sim)
include arch-sim.mk
else
$(error no valid target architcture (ARCH) specified [$(ALL_ARCHS)])
endif

# print currently used architecture
$(info current target architecture is "$(ARCH)")


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


BUILDSUBDIRS := $(patsubst %,$(BUILDDIR)/%,$(ALLSUBDIRS))
SOURCES := $(foreach d,$(ALLSUBDIRS),$(call wildcard,$d/*.c)) $(wildcard *.c)
OBJECTS := $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCES))


.PHONY: clean


$(TARGET).elf: $(OBJECTS)
	@echo LD $(NAME)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET).elf


$(OBJECTS): | $(BUILDSUBDIRS) 


$(BUILDSUBDIRS):
	@mkdir -p $@


$(BUILDDIR)/%.o: %.c
	@echo CC $<
	@$(CC) $(CFLAGS) -MD -o $@ $<
	@cp $(BUILDDIR)/$*.d $(BUILDDIR)/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' \
		< $(BUILDDIR)/$*.d >> $(BUILDDIR)/$*.P; \
	rm -f $(BUILDDIR)/$*.d

-include $(OBJECTS:%.o=%.P)


clean:
	@echo clean build directory
	@rm -rfd $(BUILDDIR)


clean-all:
	@echo clean all build directories
	@rm -rfd $(ALL_BUILDDIRS)
