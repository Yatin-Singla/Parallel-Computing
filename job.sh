#!/bin/sh
#SBATCH --time=00:10:00

gcc -o pi -fopenmp piEstimator.c
rm speedup.csv
rm precision.csv
rm iteration.csv

echo "running speedup test"
echo "running iterations on 1 thread"
./pi 1000000000 1 >> speedup.csv
echo "running iterations on 2 thread"
./pi 1000000000 2 >> speedup.csv
echo "running iterations on 4 thread"
./pi 1000000000 4 >> speedup.csv
echo "running iterations on 8 thread"
./pi 1000000000 8 >> speedup.csv
echo "running iterations on 16 thread"
./pi 1000000000 16 >> speedup.csv
echo "running iterations on 32 thread"
./pi 1000000000 32 >> speedup.csv

echo "running precision test"
echo "running 1000000000 on 1 thread"
./pi 1000000000 1 >> precision.csv
echo "running 2000000000 on 2 thread"
./pi 2000000000 2 >> precision.csv
echo "running 4000000000 on 4 thread"
./pi 4000000000 4 >> precision.csv
echo "running 8000000000 on 8 thread"
./pi 8000000000 8 >> precision.csv
echo "running 16000000000 on 16 thread"
./pi 16000000000 16 >> precision.csv
echo "running 32000000000 on 32 thread"
./pi 32000000000 32 >> precision.csv
