#!/bin/sh

#To run this script, recommended use: "sbatch -N <numberofnodes> -n <number_of_processes> <path>/sub.sh"

# slurmp job scheduler, provide upper bound of expected time for the job to run
#SBATCH --time=00:20:00


# compile code
mpicc -o a.out hypercubicpermute.c
# execute program with given size <n>
mpirun ./a.out $1




# you can add more lines of commands here, if you wish.
