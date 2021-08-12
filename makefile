GCC=cc
GFLAGS=-O3 -lm
PROG=main

all: sequential openmp setup
	$(GCC) main.c -o $(PROG) $(GFLAGS)

sequential:
	$(GCC) main.c -o $(PROG) $(GFLAGS)

openmp:
	$(GCC) main_openmp.c -o $(PROG)_openmp $(GFLAGS) -fopenmp

setup:
	mkdir -p saidas

run:
	./$(PROG)

reset:
	rm -rf saidas $(PROG)

clean:
	rm $(PROG)
