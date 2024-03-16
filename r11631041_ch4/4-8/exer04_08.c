#include <stdio.h>
#include "mpi.h"
#include "tool.h"

int main(int argc, char *argv[]) {
	int rank, size, maximum, start, end, curr, next, i, local_sum, global_sum;
	local_sum = global_sum = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	maximum = 1E6;
	get_index(rank, size, maximum, &start, &end);
	if (rank != (size - 1))
		end += 2;
	//printf("%d: %d~%d\n", rank, start ,end);

	curr = 0;
	for (i = start; i <= end; i += 2) {
		next = is_prime(i);
		local_sum += curr & next;
		curr = next;
	}

	MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Finalize();
	if (rank == 0) {
		printf("--------------------------------------------------------\n"
		       "The number of times that consecutive odd numbers between\n"
		       "2 and %d (inclusive) are both prime is:  %d\n"
		       "--------------------------------------------------------\n"
		       "\n",
		       maximum, global_sum);
	}
	return 0;
}
