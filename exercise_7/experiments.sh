#!/bin/bash

make

rm -rf plots


EXEC=./src/my_program
OUTFILE="results.csv"
echo "n,threads,row_or_col,time" > "$OUTFILE"

for ((n=10000; n<=20000; n+=1000)); do

    for row_or_col in 0 1; do
        echo "Running serial dimension=$n row_or_col=$row_or_col"
        line=$($EXEC -d $n -sp 0 -rc $row_or_col -t 1)
        echo "$line" >> "$OUTFILE"
    done

    for threads in 2 4 6 8; do
        for row_or_col in 0 1; do
            echo "Running parallel dimension=$n row_or_col=$row_or_col threads=$threads"
            line=$($EXEC -d $n -sp 1 -rc $row_or_col -t $threads)
            echo "$line" >> "$OUTFILE"
        done
    done
done

python3 plot_results.py "$OUTFILE"

make clean
