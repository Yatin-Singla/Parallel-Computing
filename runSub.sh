# $3 = N, $4 = G
# N = number of nodes
# n = number of processes
sbatch -N $1 -n $2 sub.sh $3 $4
