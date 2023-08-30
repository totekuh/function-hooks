CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared
SRC_DIR = ./src
LIB_DIR = ./lib

all: $(LIB_DIR)/hooks.so

$(LIB_DIR)/common.so: $(SRC_DIR)/common.c $(SRC_DIR)/common.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/common.c -o $(LIB_DIR)/common.so -ldl


$(LIB_DIR)/recvmsg.so: $(SRC_DIR)/recvmsg.c $(SRC_DIR)/common.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/recvmsg.c $(SRC_DIR)/common.c -o $(LIB_DIR)/recvmsg.so -ldl

$(LIB_DIR)/recvfrom.so: $(SRC_DIR)/recvfrom.c $(SRC_DIR)/common.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/recvfrom.c $(SRC_DIR)/common.c -o $(LIB_DIR)/recvfrom.so -ldl

$(LIB_DIR)/memcpy.so: $(SRC_DIR)/memcpy.c $(SRC_DIR)/common.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/memcpy.c $(SRC_DIR)/common.c -o $(LIB_DIR)/memcpy.so -ldl


$(LIB_DIR)/hooks.so: $(LIB_DIR)/common.so $(LIB_DIR)/recvmsg.so $(LIB_DIR)/recvfrom.so $(LIB_DIR)/memcpy.so
	$(CC) $(LDFLAGS) -o $(LIB_DIR)/hooks.so $(LIB_DIR)/common.so $(LIB_DIR)/recvmsg.so $(LIB_DIR)/recvfrom.so $(LIB_DIR)/memcpy.so -ldl

clean:
	rm -f $(LIB_DIR)/*.so tests/test_recvmsg tests/test_recvfrom tests/test_memcpy

test_recvmsg: tests/test_recvmsg.c
	$(CC) -o tests/test_recvmsg tests/test_recvmsg.c -lpthread

test_recvfrom: tests/test_recvfrom.c
	$(CC) -o tests/test_recvfrom tests/test_recvfrom.c -lpthread

test_memcpy: tests/test_memcpy.c
	$(CC) -o tests/test_memcpy tests/test_memcpy.c

tests: $(LIB_DIR)/hooks.so test_recvmsg test_recvfrom test_memcpy
	export LD_PRELOAD=$(LIB_DIR)/hooks.so && \
	./tests/test_recvmsg && \
	./tests/test_recvfrom && \
	./tests/test_memcpy && \
	echo -e "\033[0;32mAll tests passed\033[0m"
