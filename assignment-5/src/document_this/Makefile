FILES := calculations.c system.c main.c

BUILD := build
OBJ := $(FILES:%.c=$(BUILD)/%.o)

SRC := source
CFILES := $(FILES:%.c=$(SRC)/%.c)

CC := gcc
CFLAGS := -O0 -g3 -Wall -Werror

DEST = $(BUILD)/main
.DEFAULT_GOAL := $(DEST)

$(DEST) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(BUILD) :
	mkdir $(BUILD)

$(BUILD)/%.o : $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(.DEFAULT_GOAL) $(BUILD)