TARGET = main.out
HDRS_DIR = include

CFLAGS = -lc

SRCS = \
       main.c \
       src/message.c \

.PHONY: all clean

all: $(SRCS)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)

