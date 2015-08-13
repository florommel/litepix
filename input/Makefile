# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

NAME = litepix-input

# build directoriy
BUILDDIR := build

# include architecture specific mk-file
include arch-avr.mk


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCES))


.PHONY: clean


$(TARGET).elf: $(OBJECTS)
	@echo LD $(NAME)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET).elf


$(OBJECTS): | $(BUILDDIR) 


$(BUILDDIR):
	@mkdir -p $@


$(BUILDDIR)/%.o: %.cpp
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
