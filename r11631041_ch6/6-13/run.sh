#!/bin/sh
#PBS -P ACD112153
#PBS -N assignment
#PBS -q ctest
#PBS -l select=1:ncpus=1:mpiprocs=10
#PBS -l place=scatter
#PBS -l walltime=00:01:00
#PBS -j n
module purge
module load intel/2018_u1
cd $PBS_O_WORKDIR
date
rm assignment.* >/dev/null 2>&1
program=exer06_13
matrix_file_name=file.txt
mpicxx ${program}.c -o $program
mpiexec ./$program $matrix_file_name 10 1
