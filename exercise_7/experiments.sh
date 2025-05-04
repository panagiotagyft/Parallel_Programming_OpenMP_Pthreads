#!/bin/bash

# Μεταγλώττιση
make

# Καθαρισμός φακέλου plots
rm -rf plots

# Εκτελέσιμο πρόγραμμα
EXEC=./src/my_program

OUTFILE="results.csv"
echo "n,threads,layout,time" > "$OUTFILE"

# Βρόχος για n = 10^4 έως 10^5
for ((n=10000; n<=20000; n+=1000)); do
    # Σειριακή εκτέλεση (1 thread)
    for layout in 0 1; do
        echo "Running serial n=$n layout=$layout"
        line=$($EXEC $n 0 $layout 1)
        echo "$line" >> "$OUTFILE"
    done

    # Παράλληλη εκτέλεση
    for threads in 2 4 6 8; do
        for layout in 0 1; do
            echo "Running parallel n=$n layout=$layout threads=$threads"
            line=$($EXEC $n 1 $layout $threads)
            echo "$line" >> "$OUTFILE"
        done
    done
done

python3 plot_results.py "$OUTFILE"

make clean