#include <stdio.h>
#include <mpi.h>
#define N 50

double f(int i) {
	double x;
	x = (double)i / (double)N;
	return 4.0 / (1.0 + x * x);
}

int main() {
	int rank, size, i;
	double local_sum, global_sum, t1, t2;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	local_sum = 0;

	t1 = MPI_Wtime();
	for (i = rank + 1; i <= N / 2; i += size) {
		local_sum += 4.0 * f(2 * i - 1) + 2.0 * f(2 * i);
	}

	MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Finalize();
	
	global_sum += f(0) - f(N);
	global_sum /= 3 * N;
	t2 = MPI_Wtime();
	
	if (rank == 0) {
		FILE* file = fopen("file.csv", "a");
		fprintf(file, "%d,%.15f\n", size, t2 - t1);
		printf("The value calculated by Simpson's rule is %.15f\n", global_sum);
		fclose(file);
	}
	return 0;
}
