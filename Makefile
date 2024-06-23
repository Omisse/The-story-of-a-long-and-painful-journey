CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

LC=clang-format
LFLAGS=-style=file:$(CLNGPATH) -i

CPPC=cppcheck
CPPCFLAGS=--enable=all --std=c11 --suppress=missingIncludeSystem --check-config

BPATH=build
SRCPATH=src
CLNGPATH:=materials/linters/.clang-format

NAMES=stack.c polishnot.c graphics.c chstack.c input.c graph.c
FILE=$(addprefix $(SRCPATH)/,$(NAMES))
OBJ=$(FILE:.c=.o)
OUT=$(BPATH)/graph

.o:.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: build clean

debug: build_debug clean run-verbose

run: build $(OUT)
	./$(OUT)

run-leaks: build $(OUT)
	leaks -atExit -- ./$(OUT)

run-verbose: build_debug $(OUT)
	./$(OUT)

build: $(OBJ)
	$(LC) $(LFLAGS) $(FILE)
	$(CPPC) $(CPPCFLAGS) $(FILE)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT)

build_debug: $(OBJ)
	$(LC) $(LFLAGS) $(FILE)
	$(CPPC) $(CPPCFLAGS) $(FILE)
	$(CC) $(CFLAGS) -g -DV $(OBJ) -o $(OUT)

clean:
	rm src/*.o
