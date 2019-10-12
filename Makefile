TARGET = main.out
HDRS_DIR = project/include

SRCS = \
	project/src/main.c \
	project/src/utils.c

.PHONY: all clean

all: $(SRCS) test
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

test:
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o test.out project/src/unit_test.c

clean:
	rm -rf $(TARGET)
