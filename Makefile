CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude

BUILD := build
LIB := $(BUILD)/libtsn.a
DEMO := $(BUILD)/tsn_demo
TEST := $(BUILD)/tsn_tests
GCL_TEST := $(BUILD)/test_gcl

CORE_SRC := $(wildcard src/*.c)
CORE_OBJ := $(patsubst src/%.c,$(BUILD)/%.o,$(CORE_SRC))

.PHONY: all demo test gcl-test clean

all: demo test gcl-test

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB): $(CORE_OBJ)
	ar rcs $@ $^

$(DEMO): examples/tsn_demo.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@

$(TEST): tests/test_tsn.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@

$(GCL_TEST): tests/test_gcl.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) -o $@

demo: $(DEMO)
	./$(DEMO)

test: $(TEST)
	./$(TEST)

gcl-test: $(GCL_TEST)
	./$(GCL_TEST)

clean:
	rm -rf $(BUILD)
