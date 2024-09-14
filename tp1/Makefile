ROOT_DIR_ABS := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
ROOT_DIR := $(shell realpath --relative-to $(CURDIR) $(ROOT_DIR_ABS))
BUILD    ?= $(ROOT_DIR)/build
SRC      := $(ROOT_DIR)/src

CFLAGS   := $(EXTRA) -Wall -I$(SRC) -Werror -g -O3

sources  := $(wildcard $(SRC)/*.c)
sources  := $(filter-out %main.c, $(sources))

# Pega os nomes dos arquivos remove a extensão e diretório
objects  := $(notdir $(basename $(sources)))
# Concatena .o no final
objects  := $(addsuffix .o, $(objects))
# Contatena $BUILD/ no começo
objects  := $(addprefix $(BUILD)/, $(objects))

$(shell [[ -d "$(BUILD)" ]] || mkdir -p $(BUILD))

$(ROOT_DIR)/tp1: $(SRC)/main.c $(objects)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
	rm -f $(ROOT_DIR)/tp1
	rm -f $(ROOT_DIR)/tests/runner
	rm $(ROOT_DIR)/tests/gen

.PHONY: clean

