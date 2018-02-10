# Constants

# For more verbose output, override with an empty string. e.g. `make Q=`
Q ?= @

top := .
out_dir := $(top)/bin
src_dir := $(top)/src

obj_files := $(out_dir)/main.o
obj_files += $(out_dir)/glad.o
obj_files += $(out_dir)/gl_wrapper.o
obj_files += $(out_dir)/sdl_wrapper.o
obj_files += $(out_dir)/stb_image.o

CC = gcc
CPP = g++
MKDIR = mkdir
RM = rm


CFLAGS := -Wall -g3
CFLAGS += -Iinclude

LFLAGS := -lSDL2 -ldl

# Rules
.PHONY: all
all: test

test: $(obj_files)
	$(Q)$(CPP) $(LFLAGS) $(obj_files) -o $@

$(out_dir)/%.o: $(src_dir)/%.c
	@echo "Compile: $(notdir $<)"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

$(out_dir)/%.o: $(src_dir)/%.cpp
	@echo "Compile: $(notdir $<)"
	$(Q)$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(Q)$(RM) -f $(out_dir)/*
	$(Q)$(RM) -f $(top)/test
