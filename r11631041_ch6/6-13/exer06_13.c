#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <vector>
#define ALIVE 1
#define DEAD 0
int main(int argc, char* argv[]) {
  int size, rank, m, n, j, k, root, row, col; 
  char* filename;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  root = 0;
  if (rank == size - 1) {
    if (argc != 4) {
      fprintf(stderr, "Incorrect number of command-line arguments\n");
      exit(1);
    }
    filename = argv[1];
    j = atoi(argv[2]);
    k = atoi(argv[3]);
    if (j < 1 || k < 1 || k > j) {
      fprintf(stderr, "Invalid value of command-line arguments\n");
      exit(1);
    }
    FILE* file = fopen(filename, "r");
    fscanf(file, "%d %d", &m, &n);
    fclose(file);
  }
  MPI_Bcast(&m, 1, MPI_INT, size - 1, MPI_COMM_WORLD);
  MPI_Bcast(&n, 1, MPI_INT, size - 1, MPI_COMM_WORLD);
  MPI_Bcast(&j, 1, MPI_INT, size - 1, MPI_COMM_WORLD);
  MPI_Bcast(&k, 1, MPI_INT, size - 1, MPI_COMM_WORLD);
  int local_row = m / size;
  if (rank == size - 1) local_row += m % size;
  //Row adds zero padding
  int local_ghost_row = local_row + 2;
  //Col adds zero padding
  int ghost_col= n + 2;
  std::vector<std::vector<int> >curr(local_ghost_row, std::vector<int>(ghost_col, 0));
  std::vector<std::vector<int> >next(local_ghost_row, std::vector<int>(ghost_col, 0));
  if (rank == size - 1) {
    int num;
    FILE* file = fopen(filename, "r");
    fscanf(file, "%d %d", &num, &num);
    int r;
    //Read the cell matrix from file and distribute it to other processes!!!
    for (r = 0; r < size - 1; r++) {
      for (row = 1; row <= m / size; row++) {
        for (col = 1; col <= n; col++) {
          fscanf(file, "%d", &num);
          MPI_Send(&num, 1, MPI_INT, r, 0, MPI_COMM_WORLD);
        }
      }
    }
    //Read the rest numbers to last rank process
    for (row = 1; row <= local_row; row++) {
      for (col = 1; col <= n; col++) {
        fscanf(file, "%d", &curr[row][col]);
      }
    }
    fclose(file);
  } else {
    for (row = 1; row <= local_row; row++) {
      for (col = 1; col <= n; col++) {
        MPI_Recv(&curr[row][col], 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }
  }
  int upper_neighbor = (rank == 0) ? size - 1 : rank - 1;
  int lower_neighbor = (rank == size - 1) ? 0 : rank + 1;
  int gen;
  //For generations
  for (gen = 1; gen <= j; gen++) {
    if (rank == 0) {
      //Send bottom row below
      MPI_Send(&curr[local_row][0], ghost_col, MPI_INT, lower_neighbor, 0, MPI_COMM_WORLD);
      //Receiver bottom row from below
      MPI_Recv(&curr[local_row + 1][0], ghost_col, MPI_INT, lower_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == size - 1) {
      //Send top row above
      MPI_Send(&curr[1][0], ghost_col, MPI_INT, upper_neighbor, 0, MPI_COMM_WORLD);
      //Receiver top row from above
      MPI_Recv(&curr[0][0], ghost_col, MPI_INT, upper_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      MPI_Send(&curr[local_row][0], ghost_col, MPI_INT, lower_neighbor, 0, MPI_COMM_WORLD);
      MPI_Send(&curr[1][0], ghost_col, MPI_INT, upper_neighbor, 0, MPI_COMM_WORLD);
      MPI_Recv(&curr[local_row + 1][0], ghost_col, MPI_INT, lower_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&curr[0][0], ghost_col, MPI_INT, upper_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    //Display current grid on screen
    if (!(gen % k)) {
      if (rank != root)
        for (row = 1; row <= local_row; row++)
          MPI_Send(&curr[row][1], n, MPI_INT, root, 0, MPI_COMM_WORLD);
      if (rank == root) {
        printf("The %dth generation:\n", gen);
        for (row = 1; row <= local_row; row++) {
          for (col = 1; col <= n; col++)
            printf("%d ", curr[row][col]);
          printf("\n");
        }
        int source_rank;
        for (source_rank = 1; source_rank < size; source_rank++) {
          int n_recv = m / size;
          if (source_rank == size - 1) n_recv += m % size;
          std::vector<int>buff(n, 0);
          int i_recv;
          for (i_recv = 0; i_recv < n_recv; i_recv++) {
            MPI_Recv(&buff[0], n, MPI_INT, source_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int tmp;
            for (tmp = 0; tmp < buff.size(); tmp++) {
              printf("%d ", buff[tmp]);
            }
            printf("\n");
          }
        }
      } 
    }
    //Update the grid
    for (row = 1; row <= local_row; row++) {
      for (col = 1; col <= n; col++) {
        int alive_neighbors = 0;
        int irow, icol;
        for (irow = row - 1; irow <= row + 1; irow++) {
          for (icol = col -1; icol <= col + 1; icol++) {
            alive_neighbors += curr[irow][icol];
            //if ((irow != row || icol != col) && curr[irow][icol] == ALIVE) {
              //alive_neighbors++;
            //}
          }
        }
        alive_neighbors -= curr[row][col];
        if (alive_neighbors < 2)
          next[row][col] = DEAD;
        if (curr[row][col] == ALIVE && (alive_neighbors == 2 || alive_neighbors == 3))
          next[row][col] = ALIVE;
        if (alive_neighbors > 3)
          next[row][col] = DEAD;
        if (curr[row][col] == DEAD && alive_neighbors == 3)
          next[row][col] = ALIVE;
      }
    }
    for (row = 1; row <= local_row; row++)
      for (col = 1; col <= n; col++)
        curr[row][col] = next[row][col];
  }
  MPI_Finalize();
  return 0;
}
