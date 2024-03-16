#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define NUM_SAMPLES 1000000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    double s = 2.0;  // Edge length of the cube
    double d = 0.3;  // Diameter of the cylindrical hole
    double r = d / 2.0;  // Radius of the cylindrical hole

    int samples_per_process = NUM_SAMPLES / world_size;

    srand(time(NULL) + world_rank);  // Seed the random number generator

    int points_inside_cylinder = 0;

    for (int i = 0; i < samples_per_process; ++i) {
        double x = ((double)rand() / RAND_MAX) * s;
        double y = ((double)rand() / RAND_MAX) * s;
        double z = ((double)rand() / RAND_MAX) * s;

        // Check if the point is inside the cylindrical hole
        if (x <= r && y <= r && z <= r) {
            points_inside_cylinder++;
        }
    }

    int total_points_inside_cylinder;
    MPI_Reduce(&points_inside_cylinder, &total_points_inside_cylinder, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        double volume_fraction = (double)total_points_inside_cylinder / (double)NUM_SAMPLES;
        double total_volume = s * s * s * (1.0 - volume_fraction);

        printf("Volume of the remaining cube: %.5f\n", total_volume);
    }

    MPI_Finalize();

    return 0;
}
