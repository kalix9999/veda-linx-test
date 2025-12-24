TARGET = server
FLAGS = -I./lib -L./lib -lveda_test -lwiringPi -Wl,-rpath=./lib
LIB_DIR = ./lib


all: build_lib $(TARGET)


build_lib:
	$(MAKE) -C $(LIB_DIR)

$(TARGET) : main.c
	gcc main.c -o $(TARGET) $(FLAGS)




clean :
	rm -f $(TARGET)
	$(MAKE) -C $(LIB_DIR) clean