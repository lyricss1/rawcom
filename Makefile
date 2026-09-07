CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = src/rawcom.c
TARGET = rawcom.exe

all: $(TARGET)

$(TARGET):$(SRC)
	$(CC)$(CFLAGS) $(SRC) -o$(TARGET)

clean:
	del /f /q $(TARGET) 2>nul \vert{}\vert{} rm -f$(TARGET)