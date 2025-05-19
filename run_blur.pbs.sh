#!/bin/bash
#PBS -N mpi_blur_test
#PBS -l nodes=2:ppn=24
#PBS -l walltime=00:30:00
#PBS -j oe
#PBS -V

cd $PBS_O_WORKDIR
module load mpi

# Loop through ranks
for np in 2 3 4 5 6 7 8 9 10 11 12 24 36 48 60; do
    echo "Currently running on $np ranks" >> timing_results.txt
    { time mpirun -np $np ./PA3.x processedImage_${np}.jpg ; } 2>> timing_results.txt
done
