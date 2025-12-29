CC = aarch64-linux-gnu-gcc
LDFLAGS = -shared -lwiringPi
CFLAGS = -fPIC
TARGET = libveda_test.so

SRCS = jodo.c led.c buzzer.c
OBJS = $(SRCS:.c=.o)

PI_USER = suseok
PI_IP = 192.168.0.15
PI_DIR = heechang_server

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 
	ssh $(PI_USER)@$(PI_IP) "mkdir -p $(PI_DIR)/lib"
	scp $(TARGET) $(PI_USER)@$(PI_IP):/home/$(PI_USER)/$(PI_DIR)/lib

.o:.c
	gcc $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)
	ssh $(PI_USER)@$(PI_IP) "sudo rm -rf $(PI_DIR)"