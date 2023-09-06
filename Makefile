CC = gcc
CFLAGS = -Wall -fPIC -D_GNU_SOURCE
LDFLAGS = -shared -ldl
SRC_DIR = ./src
LIB_DIR = ./lib

all:
	mkdir -p $(LIB_DIR) # Automatically create lib directory
	$(MAKE) hooks

hooks: $(LIB_DIR)/hooks.so $(LIB_DIR)/recvmsg.so $(LIB_DIR)/recvfrom.so $(LIB_DIR)/memcpy.so $(LIB_DIR)/connect.so

$(LIB_DIR)/common.o: $(SRC_DIR)/common.c $(SRC_DIR)/common.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/recvmsg.o: $(SRC_DIR)/recvmsg.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/recvfrom.o: $(SRC_DIR)/recvfrom.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/memcpy.o: $(SRC_DIR)/memcpy.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/connect.o: $(SRC_DIR)/connect.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/recvmsg.so: $(LIB_DIR)/recvmsg.o $(LIB_DIR)/common.o
	$(CC) $(LDFLAGS) $^ -o $@ -ldl

$(LIB_DIR)/recvfrom.so: $(LIB_DIR)/recvfrom.o $(LIB_DIR)/common.o
	$(CC) $(LDFLAGS) $^ -o $@ -ldl

$(LIB_DIR)/memcpy.so: $(LIB_DIR)/memcpy.o $(LIB_DIR)/common.o
	$(CC) $(LDFLAGS) $^ -o $@ -ldl

$(LIB_DIR)/connect.so: $(LIB_DIR)/connect.o $(LIB_DIR)/common.o
	$(CC) $(LDFLAGS) $^ -o $@ -ldl

$(LIB_DIR)/hooks.so: $(LIB_DIR)/common.o $(LIB_DIR)/recvmsg.o $(LIB_DIR)/recvfrom.o $(LIB_DIR)/memcpy.o $(LIB_DIR)/connect.o
	$(CC) $(LDFLAGS) $^ -o $@ -ldl

clean:
	rm -f $(LIB_DIR)/*.o $(LIB_DIR)/*.so tests/test_recvmsg tests/test_recvfrom tests/test_memcpy tests/test_connect

test_recvmsg: tests/test_recvmsg.c
	$(CC) -o tests/test_recvmsg tests/test_recvmsg.c -lpthread

test_recvfrom: tests/test_recvfrom.c
	$(CC) -o tests/test_recvfrom tests/test_recvfrom.c -lpthread

test_memcpy: tests/test_memcpy.c
	$(CC) -o tests/test_memcpy tests/test_memcpy.c

test_connect: tests/test_connect.c
	$(CC) -o tests/test_connect tests/test_connect.c -lpthread

tests: $(LIB_DIR)/hooks.so test_recvmsg test_recvfrom test_memcpy test_connect
	export LD_PRELOAD=$(LIB_DIR)/hooks.so && \
	./tests/test_recvmsg && \
	./tests/test_recvfrom && \
	./tests/test_memcpy && \
	./tests/test_connect && \
	echo -e "\033[0;32mAll tests passed\033[0m"
