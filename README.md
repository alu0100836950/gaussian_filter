# Para compilar el codigo usamos make + "comando"
- secuencial -> `make SyP`
- openmp -> `make openmp`
- mpi ->


## Si queremos ejecutar el algoritmo secuencial ejecutamos:
`./syp imagen.jpg num_sal_y_pimienta`


## Si queremos ejecutar el codigo con openMP
`./syp_openmp imagen.jpg num_sal_y_pimienta`

## Para ejecutar MPI tenemos que elejir la siguiente:
` mpirun -n num_pro ./SyP_mpi imagen.jpg `
