#include <stdio.h>
#include "mpi.h"
#include "tool.h"

int main(int argc, char *argv[]) {
	int rank, size, maximum, start, end, i, local_diff, global_diff, last_prime;
	local_diff = global_diff = 0;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	maximum = 1E6;
	get_index(rank, size, maximum, &start, &end);

	last_prime = 0;
	for (i = start; i <= end; i += 2) {
		if (is_prime(i)) {
			if (last_prime) {
				if (i - last_prime > local_diff)
					local_diff = i - last_prime;
			}
			last_prime = i;
		}
	}
	
	MPI_Reduce(&local_diff, &global_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Finalize();
	if (rank == 0)
		printf("The largest gap between 2 and %d is %d\n", maximum, global_diff);
	return 0;
}
