CC       := gcc
CFLAGS   := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -g -O0 -fsanitize=address,undefined

TARGET   := chip8.out
SRCS     := main.c chip8.c
OBJDIR   := obj
OBJS     := $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c chip8.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)
