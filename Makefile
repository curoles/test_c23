CC:=/home/igor/tool/gcc-13.1.0/bindir/bin/gcc
#gcc -std=gnu17 test_c23.c
CFLAGS:=-Wall -Wextra -Werror -std=gnu2x -O3
LDFLAGS:=-Wl,-z,stack-size=1000000
BLD:=../build
THIRD:=../third

HEADERS:=c23defines.h array.inc.h smart_string.h list.h

TESTS:=c23 nullptr cleanup
TESTS+=transparent_union enum optional
TESTS+=foreachbit returnif arraylen
TESTS+=array constexpr immutable offsetof
TESTS+=string list

TESTS:= $(foreach item,$(TESTS),$(BLD)/test_$(item).o)

all: $(TESTS)
	$(CC) $(CFLAGS) $^ -o $(BLD)/test $(LDFLAGS)
	valgrind $(BLD)/test

$(BLD)/%.o: test/%.c $(HEADERS)
	@mkdir -p $(BLD)
	$(CC) $(CFLAGS) -c $< -o $@ -I.

bench_sort: bench_sort.c c23defines.h array.inc.h
	$(CC) $(CFLAGS) $< -o $(BLD)/bench_sort

.PHONY: third
third:
	@mkdir -p $(THIRD)
	wget -nd https://raw.githubusercontent.com/silentbicycle/greatest/master/greatest.h -O $(THIRD)/greatest.h
	wget -nd https://raw.githubusercontent.com/sheredom/utf8.h/master/utf8.h -O $(THIRD)/utf8.h

.PHONY: clean
clean:
	@rm -rf $(BLD)
	@rm -rf $(THIRD)
