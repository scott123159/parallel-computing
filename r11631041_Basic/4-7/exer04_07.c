#include <stdio.h>
#include "mpi.h"

int main() {
	int rank, size, i, p, result;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	i = rank + 1;
	p = size;
	MPI_Reduce(&i, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("The result of 1 + 2 + ... + %d is %d\n", size, result);
		if (result == p * (p + 1) / 2)
			printf("The result is equal to %d(%d+1)/2\n", size, size);
	}
	MPI_Finalize();
	return 0;
}
