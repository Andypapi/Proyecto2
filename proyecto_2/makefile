CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic

SRCS = main.c trie.c user_input.c

OBJS = $(SRCS:.c=.o)

TARGET = program_output

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
