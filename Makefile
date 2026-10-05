SRCS := main.c \
        utils/file_loader.c \
        utils/char_manip.c \
        utils/lexer.c \
        utils/parser.c \
        utils/status.c \
        utils/sort.c

HEADERS := $(wildcard include/*.h include/*/*.h)
INC_FLAGS := -Iinclude
STRICT_CHECK_FLAG := -Wpedantic -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -Wcast-qual -Wnull-dereference -Wstrict-prototypes -Wvla -Werror
CHECK_FLAG := -Wpedantic -Wall -Wextra -Werror

.PHONY: all release debug

all: release debug

release: $(SRCS) $(HEADERS)
	gcc -O2 -march=native -std=c89 $(STRICT_CHECK_FLAG) $(INC_FLAGS) $(SRCS)  -o ./output/main

test: $(SRCS) $(HEADERS)
	gcc -O0 -march=native -std=c89 $(CHECK_FLAG) $(INC_FLAGS) $(SRCS)  -o ./output/main_test

strict_test: $(SRCS) $(HEADERS)
	gcc -O0 -march=native -std=c89 $(STRICT_CHECK_FLAG) $(INC_FLAGS) $(SRCS)  -o ./output/main_test
