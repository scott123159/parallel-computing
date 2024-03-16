#!/bin/sh
#PBS -P ACD110146
#PBS -N block_decomp
#PBS -q ctest
#PBS -l select=1:ncpus=10:mpiprocs=10
#PBS -l place=scatter
#PBS -l walltime=00:01:00
#PBS -j n
module purge
module load intel/2018_u1
cd $PBS_O_WORKDIR
cat $PBS_NODEFILEx
echo $PBS_O_WORKDIR

date
mpirun -n 1 ./block_decomp ./matrix ./vector
mpirun -n 2 ./block_decomp ./matrix ./vector
mpirun -n 3 ./block_decomp ./matrix ./vector
mpirun -n 4 ./block_decomp ./matrix ./vector
mpirun -n 5 ./block_decomp ./matrix ./vector
mpirun -n 6 ./block_decomp ./matrix ./vector
mpirun -n 7 ./block_decomp ./matrix ./vector
mpirun -n 8 ./block_decomp ./matrix ./vector
mpirun -n 9 ./block_decomp ./matrix ./vector
mpirun -n 10 ./block_decomp ./matrix ./vector