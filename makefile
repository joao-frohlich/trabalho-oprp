GCC=cc
GFLAGS=-O2 -lm
PROG=main

all: main.c
	$(GCC) main.c -o $(PROG) $(GFLAGS)

run:
	./$(PROG)

clean:
	rm $(PROG)
