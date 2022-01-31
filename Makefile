.PHONY: all
all: example example2

.PHONY: example
example:
	$(CC) -o example AMException.c example.c

.PHONY: example2
example2:
	$(CC) -o example2 AMException.c example2.c
