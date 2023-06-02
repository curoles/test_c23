CC:=/home/igor/tool/gcc-13.1.0/bindir/bin/gcc
#gcc -std=gnu17 test_c23.c
CFLAGS:=-Wall -Werror -std=gnu2x
BLD:=../build

TESTS:=c23 nullptr cleanup
TESTS+=transparent_union enum optional
TESTS+=foreachbit returnif arraylen
TESTS+=array constexpr immutable offsetof

TESTS:= $(foreach item,$(TESTS),$(BLD)/test_$(item).o)

all: $(TESTS)
	$(CC) $(CFLAGS) $^ -o $(BLD)/test
	valgrind $(BLD)/test

$(BLD)/%.o: %.c mydefines.h
	@mkdir -p $(BLD)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf $(BLD)
