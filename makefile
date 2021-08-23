GCC=cc
GFLAGS=-O3 -lm
NVCC=nvcc
NFLAGS=-O3,-lm,-fopenmp
PROG=main

all: sequential openmp cuda setup

sequential:
	$(GCC) main_seq.c -o $(PROG) $(GFLAGS)

openmp:
	$(GCC) main_openmp.c -o $(PROG)_openmp $(GFLAGS) -fopenmp

cuda:
	$(NVCC) main_cuda.cu -o $(PROG)_cuda -gencode arch=compute_61,code=sm_61 -Wno-deprecated-gpu-targets -Xcompiler $(NFLAGS)

setup:
	mkdir -p saidas

run:
	./$(PROG)

reset:
	rm -rf saidas $(PROG)

clean:
	rm $(PROG)
