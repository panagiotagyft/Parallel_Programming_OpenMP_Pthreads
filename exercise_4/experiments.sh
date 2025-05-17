#!/bin/bash

# Compile the program using the Makefile
make

# Remove any existing 'plots' directory to start fresh
rm -rf plots

# Define the path to the executable
EXEC=./src/my_program

# Define the output CSV file for benchmarking results
OUTFILE="results.csv"

# Write the CSV header row
echo "barrier,threads,iterations,time" > "$OUTFILE"
echo

# Loop over problem sizes from 100,000 to 1,000,000 in steps of 100,000
for ((n=100000; n<=1000000; n+=100000)); do

    # Run the parallel version with different thread counts
    for threads in 2 4 6 8; do
        echo "Running parallel iterations=$n threads=$threads"
        # Execute the program with the given iteration count and threads,
        # capture its comma‐separated output line, and append it to the CSV
        line=$($EXEC -i $n -t $threads)
        echo "$line" >> "$OUTFILE"
    done
done

# Generate plots from the results CSV
python3 plot_results.py "$OUTFILE"

# Clean up build artifacts
make clean
