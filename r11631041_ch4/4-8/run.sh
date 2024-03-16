#!/bin/sh
#PBS -P ACD112153
#PBS -N assignment
#PBS -q ctest
#PBS -l select=1:ncpus=2:mpiprocs=100
#PBS -l place=scatter
#PBS -l walltime=00:01:00
#PBS -j n
module purge
module load intel/2018_u1
cd $PBS_O_WORKDIR
date
rm assignment.* >/dev/null 2>&1
mpicc -lm exer04_08.c tool.c -o exer04_08
mpirun ./exer04_08
