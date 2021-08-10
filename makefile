GCC=g++
GFLAGS=-O2 -std=c++17
PROG=main

all: main.cpp
	$(GCC) main.cpp -o $(PROG) $(GFLAGS)

run:
	./$(PROG)

clean:
	rm $(PROG)
