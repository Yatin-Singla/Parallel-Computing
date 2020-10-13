#!/bin/bash

# $1 = number of nodes
# $2 = number of total processes
# $3 = size of input array to program
sbatch -N $1 -n $2 sub.sh $3
