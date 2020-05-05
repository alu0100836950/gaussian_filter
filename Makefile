default: sec

SyP:
	mpicxx -o syp src/SyP.cpp `pkg-config --libs --cflags opencv4`

openmp:
	mpicxx -o syp_openmp -fopenmp src/SyP_openmp.cpp `pkg-config --libs --cflags opencv4`

mpimodify:
	mpicxx -o syp_mpi src/SyP_mpi.cpp `pkg-config --libs --cflags opencv4`
	