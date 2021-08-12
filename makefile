GCC=cc
GFLAGS=-O2 -lm
PROG=main

all: main.c setup
	$(GCC) main.c -o $(PROG) $(GFLAGS)

setup:
	mkdir -p saidas

run:
	./$(PROG)

reset:
	rm -rf saidas $(PROG)

clean:
	rm $(PROG)
