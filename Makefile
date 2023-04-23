
LIBNAME = capstone
BUILD_FOLDER = build/
SRC_FOLDER = src/

SOURCES := $(shell find $(SRC_FOLDER) -name '*.c')

call_trace: src/call_trace.c
	mkdir build
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(HEADERDIR) -I$(dir $<) $(SOURCES) -l$(LIBNAME) -o build/$@

clean:
	rm -rf build/*

format:
	clang-format-12 -i src/*

test1: test1.o
	

