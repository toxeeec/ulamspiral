CC=clang
CFLAGS=-g -Wall -Wextra -std=c17
SRC=src
OBJ=obj
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c))
TEST=test
TESTH=test.h
TESTBINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%, $(wildcard $(TEST)/*.c))
BIN=spiral

all: build

$(OBJ):
	mkdir $@

$(TEST)/bin:
	mkdir $@

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -lz $(OBJS) -o $@

$(TEST)/bin/%: $(TEST)/%.c $(OBJS) $(TEST)/$(TESTH)
	$(CC) $(CFLAGS) -lz $< $(filter-out obj/main.o, $(OBJS)) -o $@

$(TEST): $(TEST)/bin $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

build: $(BIN)

run: build
	./$(BIN)

release: CFLAGS=-o2 -DNDEBUG
release: clean
release: $(BIN)

clean:
	$(RM) -r $(BIN) $(OBJ)/* $(TEST)/bin/* *.png
