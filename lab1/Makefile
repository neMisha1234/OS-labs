CC = gcc

PARENT_BIN = main
CHILD_BIN = child

PARENT_SRC = main.c
CHILD_SRC = child.c

all: $(PARENT_BIN) $(CHILD_BIN)

$(PARENT_BIN):$(PARENT_SRC)
	$(CC) $(PARENT_SRC) -o $(PARENT_BIN)

$(CHILD_BIN):$(CHILD_SRC)
	$(CC) $(CHILD_SRC) -o $(CHILD_BIN)

clean:
	rm -f $(PARENT_BIN) $(CHILD_BIN) *.o 

.PHONY: all clean
