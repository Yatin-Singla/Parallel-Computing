#!/bin/sh

g++ -o pagerank -fopenmp PageRank.cpp node.h node.cpp
rm walkLength.txt
echo "Running pagerank on Google..."
echo "L = 100..."
./pagerank ./Data/Google_sorted.txt 100 0.1 8 >> walkLength.csv
echo "L = 200..."
./pagerank ./Data/Google_sorted.txt 200 0.1 8 >> walkLength.csv
echo "L = 400..."
./pagerank ./Data/Google_sorted.txt 400 0.1 8 >> walkLength.csv
echo "L = 800..."
./pagerank ./Data/Google_sorted.txt 800 0.1 8 >> walkLength.csv
echo "L = 1600..."
./pagerank ./Data/Google_sorted.txt 1600 0.1 8 >> walkLength.csv
echo "L = 3200..."
./pagerank ./Data/Google_sorted.txt 3200 0.1 8 >> walkLength.csv

echo "Running pagerank on BerkStan..."
echo "L = 100..."
./pagerank ./Data/Google_sorted.txt 100 0.1 8 >> walkLength.csv
echo "L = 200..."
./pagerank ./Data/Google_sorted.txt 200 0.1 8 >> walkLength.csv
echo "L = 400..."
./pagerank ./Data/Google_sorted.txt 400 0.1 8 >> walkLength.csv
echo "L = 800..."
./pagerank ./Data/Google_sorted.txt 800 0.1 8 >> walkLength.csv
echo "L = 1600..."
./pagerank ./Data/Google_sorted.txt 1600 0.1 8 >> walkLength.csv
echo "L = 3200..."
./pagerank ./Data/Google_sorted.txt 3200 0.1 8 >> walkLength.csv

echo "Running pagerank on NotreDame..."
echo "L = 100..."
./pagerank ./Data/NotreDame.txt 100 0.1 8 >> walkLength.csv
echo "L = 200..."
./pagerank ./Data/NotreDame.txt 200 0.1 8 >> walkLength.csv
echo "L = 400..."
./pagerank ./Data/NotreDame.txt 400 0.1 8 >> walkLength.csv
echo "L = 800..."
./pagerank ./Data/NotreDame.txt 800 0.1 8 >> walkLength.csv
echo "L = 1600..."
./pagerank ./Data/NotreDame.txt 1600 0.1 8 >> walkLength.csv
echo "L = 3200..."
./pagerank ./Data/NotreDame.txt 3200 0.1 8 >> walkLength.csv

echo "Running pagerank on Facebook..."
echo "L = 100..."
./pagerank ./Data/facebook_combined.txt 100 0.1 8 >> walkLength.csv
echo "L = 200..."
./pagerank ./Data/facebook_combined.txt 200 0.1 8 >> walkLength.csv
echo "L = 400..."
./pagerank ./Data/facebook_combined.txt 400 0.1 8 >> walkLength.csv
echo "L = 800..."
./pagerank ./Data/facebook_combined.txt 800 0.1 8 >> walkLength.csv
echo "L = 1600..."
./pagerank ./Data/facebook_combined.txt 1600 0.1 8 >> walkLength.csv
echo "L = 3200..."
./pagerank ./Data/facebook_combined.txt 3200 0.1 8 >> walkLength.csv
