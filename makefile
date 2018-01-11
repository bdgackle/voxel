# Constants

# For more verbose output, override with an empty string. e.g. `make Q=`
Q ?= @

top := .
out_dir := $(top)/bin
src_dir := $(top)/src

CPP = g++
MKDIR = mkdir
RM = rm


CFLAGS := -Wall -g3
LFLAGS := -lSDL2

# Rules
.PHONY: all
all: test

test: $(out_dir)/main.o
	$(Q)$(CPP) $(LFLAGS) $< -o $@

$(out_dir)/%.o: $(src_dir)/%.cpp
	@echo "Compile: $(notdir $<)"
	$(Q)$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(Q)$(RM) -f $(out_dir)/*
	$(Q)$(RM) -f $(top)/test
