#!/bin/bash

make

rm -rf plots

EXEC=./src/my_program
OUTFILE="results.csv"

echo "Threads,Type,Schedule,Chunk,Time" > "$OUTFILE"


for threads in 2 4 6 8; do
    echo "Running parallel array=10000*10000 threads=$threads"
    line=$($EXEC $threads 1000 1000)
    echo "$line" >> "$OUTFILE"
done

python3 plot_results.py $OUTFILE

make clean

