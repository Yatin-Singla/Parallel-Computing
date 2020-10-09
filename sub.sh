#!/bin/sh

#To run this script, recommended use: "sbatch -N <numberofnodes> -n <number_of_processes> <path>/sub.sh"

# setting the max wallclock time after which the job will be killed; 
# note: the format is hr:min:sec (default set here to 10 mins)

# slurmp job scheduler, provide upper bound of expected time for the job to run
#SBATCH --time=00:20:00

# compile code
mpicc -o conwayGameOfLife conwayGameOfLife.c
# this is the command to execute your parallel program
# provide N & G to Game Of Life
mpirun ./conwayGameOfLife $1 $2




# you can add more lines of commands here, if you wish.
