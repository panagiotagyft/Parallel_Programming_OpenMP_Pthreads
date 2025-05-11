#!/bin/bash

# Μεταγλώττιση
make

# Καθαρισμός φακέλου plots
rm -rf plots

# Εκτελέσιμο πρόγραμμα
EXEC=./src/my_program

OUTFILE="results.csv"

echo "barrier,threads,iterations,time" > "$OUTFILE"
echo

# Βρόχος για n = 10^4 έως 10^5
for ((n=100000; n<=1000000; n+=100000)); do
    # Σειριακή εκτέλεση (1 thread)

    # Παράλληλη εκτέλεση
    for threads in 2 4 6 8; do
        echo "Running parallel iterations=$n threads=$threads"
        line=$($EXEC -i $n -t $threads)
        echo "$line" >> "$OUTFILE"
    done
done

python3 plot_results.py "$OUTFILE"

make clean