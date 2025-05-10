#!/bin/bash

# Μεταγλώττιση
make

# Καθαρισμός φακέλου plots
rm -rf plots

# Εκτελέσιμο πρόγραμμα
EXEC=./src/my_program

OUTFILE="results.csv"
echo "n,threads,generations,time" > "$OUTFILE"

# Βρόχος για n = 10^4 έως 10^5
for n in 64 1024 4096; do
    # Σειριακή εκτέλεση (1 thread)
    
    echo "Running serial array_size=$n"
    line=$($EXEC 1000 $n 0 1)
    echo "$line" >> "$OUTFILE"

    # Παράλληλη εκτέλεση
    for threads in 2 4 6 8; do
        echo "Running parallel grid_size=$n threads=$threads"
        line=$($EXEC 1000 $n 1 $threads)
        echo "$line" >> "$OUTFILE"
    done
done

python3 plot_results.py "$OUTFILE"

make clean