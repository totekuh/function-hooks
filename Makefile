-include config.mk

LIB_DIR = ./lib
SRC_DIR = ./src

# If CROSS_COMPILE is defined, use it as a prefix for the compiler
CC = $(CROSS_COMPILE)gcc

# List all .c files from SRC_DIR
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
# Create corresponding .o filenames
OBJ_FILES = $(SRC_FILES:.c=.o)
# Create corresponding .so filenames in LIB_DIR
SO_FILES = $(patsubst $(SRC_DIR)/%.c,$(LIB_DIR)/%.so,$(SRC_FILES))

# Define linker flags variable based on CUSTOM_LIB_PATH
LDFLAGS := $(if $(CUSTOM_LIB_PATH),-L$(CUSTOM_LIB_PATH),)

.PHONY: all menuconfig clean

all: $(SO_FILES)


# Compile common.c into common.o
$(SRC_DIR)/common.o: $(SRC_DIR)/common.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to make each .so file, also linking in common.o
$(SO_FILES): $(LIB_DIR)/%.so : $(SRC_DIR)/%.o $(SRC_DIR)/common.o
	mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -static-libgcc -shared -fPIC -o $@ $^ $(LDFLAGS) -ldl

# Generic rule for building object files
$(OBJ_FILES): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@


menuconfig:
	./config.sh

clean:
	rm -rf $(LIB_DIR)
	rm -f $(SRC_DIR)/*.o
