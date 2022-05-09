CC=clang
CFLAGS=-g -Wall -Wextra -std=c17
SRC=src
OBJ=obj
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c))
TEST=test
TESTH=test.h
TESTBINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%, $(wildcard $(TEST)/*.c))
BIN=bin

all: run

$(BIN):
	mkdir $@

$(OBJ):
	mkdir $@

$(TEST)/bin:
	mkdir $@

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN)/main: $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) -lz $(OBJS) -o $@

$(TEST)/bin/%: $(TEST)/%.c $(OBJS) $(TEST)/$(TESTH)
	$(CC) $(CFLAGS) $< $(filter-out obj/main.o, $(OBJS)) -o $@

$(TEST): $(TEST)/bin $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

run: $(BIN)/main
	./$^

release: CFLAGS=-o2 -DNDEBUG
release: clean
release: $(BIN)/main

clean:
	$(RM) -r $(BIN)/* $(OBJ)/* $(TEST)/bin/*
