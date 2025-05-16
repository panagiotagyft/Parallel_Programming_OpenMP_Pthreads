#!/bin/bash

make

rm -rf plots

EXEC=./src/my_program
OUTFILE="results.csv"

echo "n,threads,time" > "$OUTFILE"

for ((n=10000000; n<=100000000; n+=10000000)); do
    
    echo "Running serial array_size=$n"
    line=$($EXEC -s $n -sp 0 -t 1)
    echo "$line" >> "$OUTFILE"

    for threads in 2 4 6 8; do
        echo "Running parallel array_size=$n threads=$threads"
        line=$($EXEC -s $n -sp 1 -t $threads)
        echo "$line" >> "$OUTFILE"
    done
done

python3 plot_results.py "$OUTFILE"

make clean
