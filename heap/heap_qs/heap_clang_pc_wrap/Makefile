CC = clang
CFLAGS = -Wall -Wextra -g -O2
# WAJIB: Beritahu linker untuk membungkus fungsi malloc ke __wrap_malloc
LDFLAGS = -Wl,--wrap=malloc

TARGET = main
SRCS = main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
