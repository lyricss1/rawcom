CC = gcc
CFLAGS = -Wall -I.

TARGET = rawcom
SRCS = rawcom.c src/serial.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) $(TARGET).exe