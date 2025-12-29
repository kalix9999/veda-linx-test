
LIB_DIR = ./lib

SERVER_TARGET = server
SERVER_SRC = main.c
SERVER_CC = aarch64-linux-gnu-gcc

SERVER_FLAGS = -I$(LIB_DIR) -L$(LIB_DIR) -lveda_test -lwiringPi -Wl,-rpath=$(LIB_DIR) -pthread

CLIENT_TARGET = client
CLIENT_SRC = client.c
CLIENT_CC = gcc

CLIENT_FLAGS = -pthread 
PI_USER = suseok
PI_IP = 192.168.0.15
PI_DIR = heechang_server

all: build_lib $(SERVER_TARGET) $(CLIENT_TARGET)


build_lib:
	$(MAKE) -C $(LIB_DIR)

$(SERVER_TARGET): $(SERVER_SRC)
	$(SERVER_CC) $(SERVER_SRC) -o $(SERVER_TARGET) $(SERVER_FLAGS)
	scp $(SERVER_TARGET) $(PI_USER)@$(PI_IP):/home/$(PI_USER)/$(PI_DIR)
	



$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CLIENT_CC) $(CLIENT_SRC) -o $(CLIENT_TARGET) $(CLIENT_FLAGS)

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET) *.o
	$(MAKE) -C $(LIB_DIR) clean