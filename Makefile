
MAKEFLAGS := --jobs=$(shell nproc)

CC = cc
CFLAGS = -Wall -Wextra -O3 -std=c17 -pedantic
LDLIBS =  -lcomctl32 -mwindows

.PHONY: all
all: blox_gdi

blox_gdi: blox_gdi.o board.o game.o shape.o blox_gdi_rc.o

blox_gdi.o: blox_gdi.c board.h game.h shape.h

blox_gdi_rc.o: blox_gdi.rc
	windres $< $@

board.o: board.h

game.o: board.h game.h shape.h

shape.o: board.h shape.h

.PHONY: format
format:
	clang-format -i *.c *.h

.PHONY: run
run: blox_gdi
	./blox_gdi.exe

.PHONY: clean
clean:
	rm -f *.o *.a *.res blox_gdi.exe
