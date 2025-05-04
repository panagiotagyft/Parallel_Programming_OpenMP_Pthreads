#!/bin/bash

# Μεταγλώττιση
make

# Καθαρισμός φακέλου plots
rm -rf plots

# Εκτελέσιμο πρόγραμμα
EXEC=./src/my_program

OUTFILE="results.csv"
echo "n,threads,time" > "$OUTFILE"

# Βρόχος για n = 10^4 έως 10^5
for ((n=10000000; n<=100000000; n+=10000000)); do
    # Σειριακή εκτέλεση (1 thread)
    
    echo "Running serial array_size=$n"
    line=$($EXEC $n 0 $layout 1)
    echo "$line" >> "$OUTFILE"

    # Παράλληλη εκτέλεση
    for threads in 2 4 6 8; do
        echo "Running parallel array_size=$n threads=$threads"
        line=$($EXEC $n 1 $layout $threads)
        echo "$line" >> "$OUTFILE"
    done
done

python3 plot_results.py "$OUTFILE"

make clean