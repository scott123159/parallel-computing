#include <math.h>
#include "tool.h"

int is_prime(int n) {
	if (n < 2) return 0;
	int i;
	for (i = 2; i <= sqrt(n); i++)
		if (n % i == 0) return 0;
	return 1;
}

void get_index(int rank, int size, int maximum, int* start, int* end) {
	*start = (rank * maximum / size) + 1;
	*end = (rank + 1) * maximum / size;
	if ((*start) % 2 == 0) (*start)++;
}
