default: sec

sec:
	mpicxx -o secuencial src/prueba.cpp `pkg-config --libs --cflags opencv`

openmp:
	mpicxx -o openmp -fopenmp src/prueba.cpp `pkg-config --libs --cflags opencv`

mpi:
	mpicxx -o mpi src/prueba_mpi.cpp `pkg-config --libs --cflags opencv`




