CC:=/home/igor/tool/gcc-13.1.0/bindir/bin/gcc
#gcc -std=gnu17 test_c23.c
CFLAGS:=-Wall -Werror -std=gnu2x
BLD:=../build

TESTS:=$(BLD)/test_c23.o $(BLD)/test_nullptr.o $(BLD)/test_cleanup.o
TESTS+=$(BLD)/test_transparent_union.o $(BLD)/test_enum.o $(BLD)/test_optional.o
TESTS+=$(BLD)/test_foreachbit.o $(BLD)/test_returnif.o $(BLD)/test_arraylen.o

all: $(TESTS)
	$(CC) $(CFLAGS) $^ -o $(BLD)/test
	$(BLD)/test

$(BLD)/%.o: %.c mydefines.h
	@mkdir -p $(BLD)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf $(BLD)