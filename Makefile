CC = gcc
CFLAGS = -Wall -fPIC -I src/include
LDFLAGS =
TARGET = bin/test

# Directories
SRC_DIR = src
BIN_DIR = bin
LIB_DIR = lib
INCLUDE_DIR = src/include

# Source files for the library (exclude test.c)
LIB_SRCS = $(filter-out $(SRC_DIR)/test.c, $(wildcard $(SRC_DIR)/*.c))

# Object files for the library
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(LIB_SRCS))

# Static library name
LIBRARY = $(LIB_DIR)/libhwfp.a
# Shared library name
SHARED_LIB = $(LIB_DIR)/libhwfp.so

# Python SWIG bindings
SWIG_INTERFACE = src/interface/hwfp.i
PYTHON_LIB = _hwfp.so

# Default target
all: $(TARGET) $(LIBRARY) $(SHARED_LIB) python_bindings

# Compile test.c and link it with the library for the executable
$(TARGET): $(BIN_DIR)/test.o $(LIBRARY)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(BIN_DIR)/test.o -L$(LIB_DIR) -lhwfp -o $(TARGET)

# Compile test.c to object file
$(BIN_DIR)/test.o: $(SRC_DIR)/test.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files for the library to object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build static library
$(LIBRARY): $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $(LIBRARY) $(LIB_OBJS)

# Build shared library
$(SHARED_LIB): $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared -o $(SHARED_LIB) $(LIB_OBJS)

# Generate Python bindings using SWIG
python_bindings:
	swig -python -I$(INCLUDE_DIR) -outdir $(BIN_DIR) -o $(BIN_DIR)/hwfp_wrap.c $(SWIG_INTERFACE)
	$(CC) $(CFLAGS) `python3-config --cflags` -c $(BIN_DIR)/hwfp_wrap.c -o $(BIN_DIR)/hwfp_wrap.o
	$(CC) -shared $(BIN_DIR)/hwfp_wrap.o $(SHARED_LIB) -o $(BIN_DIR)/$(PYTHON_LIB) -L$(LIB_DIR) -lhwfp
	mkdir -p hwfp
	cp $(BIN_DIR)/hwfp.py ./hwfp/hwfp.py
	cp $(BIN_DIR)/$(PYTHON_LIB) ./hwfp/_hwfp.so

# Clean up build files
clean:
	rm -rf $(BIN_DIR)/*.o $(BIN_DIR)/*_wrap.* $(BIN_DIR)/_hwfp.so $(LIB_DIR)/*.so $(TARGET)

.PHONY: all clean python_bindings

