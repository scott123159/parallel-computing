#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main() {

  int rank, size;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status stat;

  for (int i = 0; i <= 27; i++) {
    //資料大小為8B到1GB
    #define N (1 << i)
    double* data = (double*)malloc(N * sizeof(double));
    for (int j = 0; j < N; j++)
      data[j] = (double)rand() / (double)RAND_MAX;

    //進行熱機
    for (int j = 0; j < 5; j++) {
      if (rank == 0) {
        MPI_Send(data, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(data, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &stat);
      } else if (rank == 1) {
        MPI_Recv(data, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat);
        MPI_Send(data, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      } 
    }

    //時間戳記
    double start, end, elapsed_time;    
    if (rank == 0) {
      MPI_Send(data, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
      start = MPI_Wtime();
      MPI_Recv(data, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &stat);
      end = MPI_Wtime();
    } else if (rank == 1) {
      MPI_Recv(data, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat);
      MPI_Send(data, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } 

    if (rank == 0) {
      FILE* file = fopen("file.csv", "a");
      elapsed_time = end - start;
      fprintf(file, "%d,%.15f\n", 8 * N, elapsed_time / 2);
      fclose(file);
    }
    free(data);
  } 

  MPI_Finalize();
  return 0;
}
