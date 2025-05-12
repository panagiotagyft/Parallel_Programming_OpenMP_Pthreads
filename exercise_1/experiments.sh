#!/bin/bash

# Μεταγλώττιση
make

# Καθαρισμός φακέλου plots
rm -rf plots

# Εκτελέσιμο πρόγραμμα
EXEC=./src/my_program

OUTFILE="results.csv"

echo "Implementation,threads,num_darts,arrows,time" > "$OUTFILE"
echo

# Βρόχος για n = 10^4 έως 10^5
for ((n=100000000; n<=1000000000; n+=100000000)); do

    for threads in 2 4 6 8; do
        echo "Running darts=$n threads=$threads"
        line=$($EXEC -d $n -t $threads)
        echo "$line" >> "$OUTFILE"
    done
done

python3 plot_results.py "$OUTFILE"

make clean