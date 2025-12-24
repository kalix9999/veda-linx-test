CC = gcc
LDFLAGS = -shared -lwiringPi
CFLAGS = -fPIC
TARGET = libveda_test.so

SRCS = jodo.c led.c buzzer.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 

.o:.c
	gcc $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)