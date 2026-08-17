CC       := gcc
CFLAGS   := -std=c17 -Wall -Wextra -Wpedantic -Wshadow -g -O0 -fsanitize=address,undefined

BACKEND    ?= term

ifeq ($(BACKEND),sdl)
CFLAGS     += $(shell pkg-config --cflags sdl2)
LDLIBS     += $(shell pkg-config --libs sdl2)
endif

TARGET     := chip8.out
SRCS       := main.c chip8.c platform_$(BACKEND).c
OBJDIR     := obj
OBJS       := $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: %.c chip8.h platform.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)
