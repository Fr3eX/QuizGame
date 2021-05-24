.PHONY: all ,clean,clean_test
.SUFFIXES:



CC=gcc
CFLAGS=`mysql_config --cflags`
LDFLAGS=`mysql_config --libs`
DEBUG=

RM=rm

SRC=src
OBJ=obj
TEST=tests
BIN_DIR=bin
TEST_BINDIR=$(TEST)/$(BIN_DIR)

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
TESTS=$(wildcard $(TEST)/$(SRC)/*.c)
TEMP=$(filter-out $(OBJ)/main.o,$(OBJS))

TEST_BINS=$(patsubst $(TEST)/$(SRC)/%.c,$(TEST_BINDIR)/%,$(TESTS))
BIN=$(BIN_DIR)/prog


all:$(BIN)

release: CFLAGS=-Wall -O2 -DNDEBUF
release: clean
release: $(BIN)



test: $(TEMP) $(TEST_BINS)
	for test in $(TEST_BINS);do ./$$test --verbose -j1  ;done

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ)/%.o:$(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BINDIR)/%:$(TEST)/$(SRC)/%.c
	$(CC) $(CFLAGS) $< $(TEMP) -o $@ $(LDFLAGS) -lcriterion	

clean_test:
	$(RM) $(TEST_BINS)
clean:
	$(RM) $(OBJS) $(BIN)
