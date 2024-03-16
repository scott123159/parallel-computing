rm -f block_decomp
mpicc gen-vector.c -o gen-vector
./gen-vector 100 vector
mpicc gen-int-matrix.c -o gen-int-matrix
./gen-int-matrix 100 matrix
mpicc -o block_decomp block_decomp.c MyMPI.c
mpirun -n 1 ./block_decomp ./matrix ./vector