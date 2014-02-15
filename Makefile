#-*- mode:makefile-gmake; -*-
ROOT = $(shell pwd)
TARGET = cgc

INCLUDE += -I$(ROOT)/src
SOURCES = $(wildcard $(ROOT)/src/*.c)

OBJS = $(patsubst %.c,%.o,$(SOURCES))
CPPFLAGS = $(OPTCPPFLAGS)
LIBS = -lm $(OPTLIBS)
CFLAGS = -g -std=c99 $(INCLUDE) -Wall -Werror $(OPTFLAGS)

ifeq ($(shell uname),Darwin)
LIB_TARGET = libcgc.dylib
else
LIB_TARGET = libcgc.so
endif

.DEFAULT_GOAL = all
all: $(TARGET) $(LIB_TARGET)

clean:
	make -C tests clean
	rm -rf $(OBJS) $(TARGET) $(TARGET).o $(TARGET).new $(LIB_TARGET)

test:
	@make -C tests clean test

.PHONY: all clean test libcgc

$(TARGET): $(TARGET).o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@.new
	mv $@.new $@

$(LIB_TARGET): $(OBJS)
	$(CC) -shared $(OBJS) $(LIBS) -o $(LIB_TARGET)

libcgc: $(LIB_TARGET)

%.o: %.c
	$(CC) -fPIC $(CFLAGS) -o $@ -c $^
