#!/bin/sh

g++ -o pagerank -fopenmp PageRank.cpp node.h node.cpp
rm runtime.csv
echo "Running pagerank on Google..."
echo "1 threads..."
./pagerank ./Data/Google_sorted.txt 500 0.1 1 >> runtime.csv
echo "2 threads..."
./pagerank ./Data/Google_sorted.txt 500 0.1 2 >> runtime.csv
echo "4 threads..."
./pagerank ./Data/Google_sorted.txt 500 0.1 4 >> runtime.csv
echo "8 threads..."
./pagerank ./Data/Google_sorted.txt 500 0.1 8 >> runtime.csv

echo "Running pagerank on BerkStan..."
echo "1 threads..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.1 1 >> runtime.csv
echo "2 threads..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.1 2 >> runtime.csv
echo "4 threads..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.1 4 >> runtime.csv
echo "8 threads..."
./pagerank ./Data/BerkStan_sorted.txt 500 0.1 8 >> runtime.csv

echo "Running pagerank on NotreDame..."
echo "1 threads..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.1 1 >> runtime.csv
echo "2 threads..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.1 2 >> runtime.csv
echo "4 threads..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.1 4 >> runtime.csv
echo "8 threads..."
./pagerank ./Data/NotreDame_sorted.txt 500 0.1 8 >> runtime.csv

echo "Running pagerank on Facebook..."
echo "1 threads..."
./pagerank ./Data/facebook_combined.txt 500 0.1 1 >> runtime.csv
echo "2 threads..."
./pagerank ./Data/facebook_combined.txt 500 0.1 2 >> runtime.csv
echo "4 threads..."
./pagerank ./Data/facebook_combined.txt 500 0.1 4 >> runtime.csv
echo "8 threads..."
./pagerank ./Data/facebook_combined.txt 500 0.1 8 >> runtime.csv
