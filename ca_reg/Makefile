CFLAGS=-g -O2 -Wall --std=c99
TARGETS=ca_serial ca_pthreads ca_mpi ca_mpi_omp

all: $(TARGETS)

ca_serial: ca_serial.c
	gcc $(CFLAGS) -o $@ $<

ca_pthreads: ca_pthreads.c
	gcc -g -O2 --std=gnu99 -Wno-unknown-pragmas -Wall -o $@ $< -lpthread

ca_mpi: ca_mpi_omp.c
	mpicc $(CFLAGS) -o $@ $<

ca_mpi_omp: ca_mpi_omp.c
	mpicc $(CFLAGS) -o $@ $< -fopenmp

clean:
	rm -f $(TARGETS)
