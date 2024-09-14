EXTRA := -DMACHINE -lm
include ../Makefile
TEST_DIR := $(ROOT_DIR)/tests

runner: $(TEST_DIR)/runner
gen: $(TEST_DIR)/gen

$(TEST_DIR)/runner: $(SRC)/main.c $(objects)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_DIR)/gen: $(TEST_DIR)/gen.c $(objects)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: runner gen

.DEFAULT_GOAL := $(ROOT_DIR)/tests/runner
