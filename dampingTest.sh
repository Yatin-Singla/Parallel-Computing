#!/bin/sh

g++ -o pagerank -fopenmp PageRank.cpp node.h node.cpp
rm damping.csv
echo "Running pagerank on Google..."
echo "D = 0.1..."
./pagerank ./Data/Google_sorted.txt 500 0.1 8 >> damping.csv
echo "D = 0.3..."
./pagerank ./Data/Google_sorted.txt 500 0.3 8 >> damping.csv
echo "D = 0.5..."
./pagerank ./Data/Google_sorted.txt 500 0.5 8 >> damping.csv
echo "D = 0.7..."
./pagerank ./Data/Google_sorted.txt 500 0.7 8 >> damping.csv
echo "D = 0.9..."
./pagerank ./Data/Google_sorted.txt 500 0.9 8 >> damping.csv

echo "Running pagerank on BerkStan..."
echo "D = 0.1..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.1 8 >> damping.csv
echo "D = 0.3..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.3 8 >> damping.csv
echo "D = 0.5..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.5 8 >> damping.csv
echo "D = 0.7..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.7 8 >> damping.csv
echo "D = 0.9..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.9 8 >> damping.csv

echo "Running pagerank on NotreDame..."
echo "D = 0.1..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.1 8 >> damping.csv
echo "D = 0.3..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.3 8 >> damping.csv
echo "D = 0.5..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.5 8 >> damping.csv
echo "D = 0.7..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.7 8 >> damping.csv
echo "D = 0.9..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.9 8 >> damping.csv

echo "Running pagerank on Facebook..."
echo "D = 0.1..."
./pagerank ./Data/facebook_combined.txt 500 0.1 8 >> damping.csv
echo "D = 0.3..."
./pagerank ./Data/facebook_combined.txt 500 0.3 8 >> damping.csv
echo "D = 0.5..."
./pagerank ./Data/facebook_combined.txt 500 0.5 8 >> damping.csv
echo "D = 0.7..."
./pagerank ./Data/facebook_combined.txt 500 0.7 8 >> damping.csv
echo "D = 0.9..."
./pagerank ./Data/facebook_combined.txt 500 0.9 8 >> damping.csv