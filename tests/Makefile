CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -O2 -g
INCLUDES= -I.. -I.
LDFLAGS =

# Adjust TRIM_SRC if your trim.c lives elsewhere
TRIM_SRC = ../trim.c
TEST_SRC = test_trim.c
TARGET   = test_trim

all: $(TARGET)

$(TARGET): $(TEST_SRC) $(TRIM_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(TEST_SRC) $(TRIM_SRC) $(LDFLAGS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all test clean