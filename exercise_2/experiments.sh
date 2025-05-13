#!/bin/bash

# Compile the program
make

# Remove any existing plots directory
rm -rf plots

# Path to the executable
EXEC=./src/my_program

# CSV file to store the results
OUTFILE="results.csv"

# Write CSV header
echo "lock,threads,iterations,time" > "$OUTFILE"
echo

# Loop over iteration counts from 100 000 000 to 500 000 000, in steps of 100 000 000
for ((n=100000000; n<=500000000; n+=100000000)); do

    # Parallel execution with different thread counts
    for threads in 2 4 6 8; do
        echo "Running parallel iterations=$n threads=$threads"
        # Run the program with -i (iterations) and -t (threads) and capture its output
        line=$($EXEC -i $n -t $threads)
        # Append the output line (expected in CSV format) to the results file
        echo "$line" >> "$OUTFILE"
    done

done

# Generate plots from the collected data
python3 plot_results.py "$OUTFILE"

# Clean up all build artifacts
make clean
