CC = clang
CFLAGS = -Wall -Wextra -O2
# Beritahu linker untuk membungkus fungsi malloc
LDFLAGS = -Wl,--wrap=malloc

TARGET = main
SRCS = main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
