#!/bin/sh

g++ -o pagerank -fopenmp PageRank.cpp node.h node.cpp
rm scaling.csv

echo "Running Pagerank on Google..."
echo "L = 100 p = 1..."
./pagerank ./Data/Google_sorted.txt 100 0.1 1 >> scaling.csv
echo "L = 200 p = 1..."
./pagerank ./Data/Google_sorted.txt 200 0.1 1 >> scaling.csv
echo "L = 400 p = 1..."
./pagerank ./Data/Google_sorted.txt 400 0.1 1 >> scaling.csv
echo "L = 800 p = 1..."
./pagerank ./Data/Google_sorted.txt 800 0.1 1 >> scaling.csv
echo "L = 1600 p = 1..."
./pagerank ./Data/Google_sorted.txt 1600 0.1 1 >> scaling.csv
echo "L = 3200 p = 1..."
./pagerank ./Data/Google_sorted.txt 3200 0.1 1 >> scaling.csv

echo "L = 100 p = 2..."
./pagerank ./Data/Google_sorted.txt 100 0.1 2 >> scaling.csv
echo "L = 200 p = 2..."
./pagerank ./Data/Google_sorted.txt 200 0.1 2 >> scaling.csv
echo "L = 400 p = 2..."
./pagerank ./Data/Google_sorted.txt 400 0.1 2 >> scaling.csv
echo "L = 800 p = 2..."
./pagerank ./Data/Google_sorted.txt 800 0.1 2 >> scaling.csv
echo "L = 1600 p = 2..."
./pagerank ./Data/Google_sorted.txt 1600 0.1 2 >> scaling.csv
echo "L = 3200 p = 2..."
./pagerank ./Data/Google_sorted.txt 3200 0.1 2 >> scaling.csv

echo "L = 100 p = 4..."
./pagerank ./Data/Google_sorted.txt 100 0.1 4 >> scaling.csv
echo "L = 200 p = 4..."
./pagerank ./Data/Google_sorted.txt 200 0.1 4 >> scaling.csv
echo "L = 400 p = 4..."
./pagerank ./Data/Google_sorted.txt 400 0.1 4 >> scaling.csv
echo "L = 800 p = 4..."
./pagerank ./Data/Google_sorted.txt 800 0.1 4 >> scaling.csv
echo "L = 1600 p = 4..."
./pagerank ./Data/Google_sorted.txt 1600 0.1 4 >> scaling.csv
echo "L = 3200 p = 4..."
./pagerank ./Data/Google_sorted.txt 3200 0.1 4 >> scaling.csv

echo "L = 100 p = 8..."
./pagerank ./Data/Google_sorted.txt 100 0.1 8 >> scaling.csv
echo "L = 200 p = 8..."
./pagerank ./Data/Google_sorted.txt 200 0.1 8 >> scaling.csv
echo "L = 400 p = 8..."
./pagerank ./Data/Google_sorted.txt 400 0.1 8 >> scaling.csv
echo "L = 800 p = 8..."
./pagerank ./Data/Google_sorted.txt 800 0.1 8 >> scaling.csv
echo "L = 1600 p = 8..."
./pagerank ./Data/Google_sorted.txt 1600 0.1 8 >> scaling.csv
echo "L = 3200 p = 8..."
./pagerank ./Data/Google_sorted.txt 3200 0.1 8 >> scaling.csv
