.PHONY: all
all: example example2

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: example
example: AMException.o example.o
	$(CC) -o example AMException.o example.o

.PHONY: example2
example2: AMException.o example2.o
	$(CC) -o example2 AMException.o example2.o

