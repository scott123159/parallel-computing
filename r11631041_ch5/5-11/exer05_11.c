#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
	int rank, size, n, d, i;
	double local_sum, global_sum, t1, t2;
	local_sum = global_sum = 0;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (!rank) {
		if (argc != 3) {
			fprintf(stderr, "You need to provide two parameters n and d\n");
			return 1;
		}
		n = atoi(argv[1]);
		d = atoi(argv[2]);
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);

	t1 = MPI_Wtime();
	for (i = rank + 1; i <= n; i += size) local_sum += 1.0 * pow(10, d + 5) / i;
	t2 = MPI_Wtime();
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Finalize();
	global_sum /= pow(10, 5);

	if (!rank) {
		FILE* file = fopen("file.csv", "a");
                fprintf(file, "%d,%.15f\n", size, t2 - t1);
		fclose(file);
		char str[1024];
		sprintf(str, "%.f", global_sum);
		printf("The value of Sn for n = %d and precision d = %d is ", n, d);
		int k;
		for (k = 0; k < strlen(str); k++) {
			if (k == strlen(str) - d)
				printf(".");
			else
				printf("%c", str[k]);
		}
		printf("\n");
	}
	return 0;
}
