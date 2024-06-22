CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

LC=clang-format
LFLAGS=-style=file:$(CLNGPATH) -i

CPPC=cppcheck
CPPCFLAGS=--enable=all --std=c11 --suppress=missingIncludeSystem --check-config

BPATH=build
SRCPATH=src
CLNGPATH:=materials/linters/.clang-format

#Q1_NAMES=dmanager_module.c
#Q1_FILE=$(addprefix $(SRCPATH)/,$(Q1_NAMES))
#Q1_OBJ=$(Q1_FILE:.c=.o)
#Q1_OUT=$(BPATH)/Quest_1

T_NAMES=graphics.c graph.c
T_FILE=$(addprefix $(SRCPATH)/,$(T_NAMES))
T_OBJ=$(T_FILE:.c=.o)
T_OUT=$(BPATH)/test.exe

.o:.c
	$(CC) $(CFLAGS) -c $^ -o $@

test: test_run test_clean

test_build: $(T_OBJ)
	$(LC) $(LFLAGS) $(T_FILE)
	$(CPPC) $(CPPCFLAGS) $(T_FILE)
	$(CC) $(CFLAGS) $(T_OBJ) -o $(T_OUT)

test_clean: $(T_OBJ)
	rm $^

test_run_leaks: test_build
	leaks -atExit -- ./$(T_OUT)

test_run: test_build
	./$(T_OUT)
