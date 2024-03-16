#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

typedef unsigned long long int uint64;

//確認是否為perfect number
int is_perfect(uint64 number) {
    uint64 divisors_sum = 1;  // 從 1 開始，因為每個數字都能被 1 整除
    uint64 sqrt_number = (uint64)sqrt(number);

    for (uint64 i = 2; i <= sqrt_number; ++i) {
        if (number % i == 0) {
            divisors_sum += i;
            if (i != number / i) {
                divisors_sum += number / i;
            }
        }
    }

    return (divisors_sum == number);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    uint64 numbers_per_process = 100000000000;
    uint64 start_number = world_rank * numbers_per_process + 2;  // 從 2 開始，因為 1 不是完美數
    uint64 end_number = (world_rank + 1) * numbers_per_process;

    uint64 perfect_numbers[8] = {};
    uint64 perfect_count = 0;

    for (uint64 num = start_number; num <= end_number && perfect_count < 8; ++num) {
        if (is_perfect(num)) {
            pruint64f("%d: %llu\r\n", world_rank, num);
            perfect_numbers[perfect_count++] = num;
        }
    }

    // 將所有結果收集起來
    uint64 all_perfect_numbers[8 * world_size];
    MPI_Gather(perfect_numbers, 8, MPI_INT, all_perfect_numbers, 8, MPI_INT, 0, MPI_COMM_WORLD);

    // 印結果
    //if (world_rank == 0) {
        //pruint64f("前八個完美數：");
        //for (uint64 i = 0; i < 8; ++i) {
            //pruint64f("%d ", all_perfect_numbers[i]);
        //}
        //pruint64f("\n");
    //}

    MPI_Finalize();

    return 0;
}
