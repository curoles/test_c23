CC:=/home/igor/tool/gcc-13.1.0/bindir/bin/gcc
#gcc -std=gnu17 test_c23.c
CFLAGS:=-Wall -Wextra -Werror -std=gnu2x -O3
LDFLAGS:=-Wl,-z,stack-size=1000000
BLD:=../build

TESTS:=c23 nullptr cleanup
TESTS+=transparent_union enum optional
TESTS+=foreachbit returnif arraylen
TESTS+=array constexpr immutable offsetof

TESTS:= $(foreach item,$(TESTS),$(BLD)/test_$(item).o)

all: $(TESTS)
	$(CC) $(CFLAGS) $^ -o $(BLD)/test $(LDFLAGS)
	valgrind $(BLD)/test

$(BLD)/%.o: %.c mydefines.h array.inc.h
	@mkdir -p $(BLD)
	$(CC) $(CFLAGS) -c $< -o $@

bench_sort: bench_sort.c mydefines.h array.inc.h
	$(CC) $(CFLAGS) $< -o $(BLD)/bench_sort

.PHONY: clean
clean:
	@rm -rf $(BLD)
