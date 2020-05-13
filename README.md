# Para compilar el codigo usamos make + "comando"
- secuencial -> `make sec`
- openmp -> `make openmp`
- mpi -> `make mpi`


## Si queremos ejecutar el algoritmo secuencial ejecutamos:
`./secuencial imagen.jpg output_image.jpg`


## Si queremos ejecutar el codigo con openMP
`./openmp imagen.jpg output_image.jpg`

## Para ejecutar MPI tenemos que elejir la siguiente:
` mpirun -n num_pro ./mpi imagen.jpg output_image.jpg`
