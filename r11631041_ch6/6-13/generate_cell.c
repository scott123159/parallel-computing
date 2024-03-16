#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  FILE* file = fopen(argv[1], "w");
  int i, j, row, col;
  srand(time(NULL));
  row = atoi(argv[2]);
  col = atoi(argv[3]);
  fprintf(file, "%d %d\n", row, col);
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++)
      fprintf(file, "%d ", rand() % 2);
    fprintf(file, "\n");
  }
  fclose(file);
  return 0;
}
