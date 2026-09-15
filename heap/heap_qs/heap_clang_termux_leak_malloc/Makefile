CC = clang
CFLAGS = -Wall -Wextra -g -fsanitize=address
LDFLAGS = -fsanitize=address

TARGET = main
SRCS = main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
