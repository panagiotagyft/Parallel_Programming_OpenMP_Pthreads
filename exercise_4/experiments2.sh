#!/bin/bash

# Compile the program using the Makefile
make

# Remove any existing 'plots' directory to start fresh
rm -rf plots

# Define the path to the executable
EXEC=./src/my_program

# Define the output CSV file for benchmarking results
OUTFILE="results2.csv"

# Write the CSV header row
echo "barrier,threads,iterations,time" > "$OUTFILE"
echo


# Run the parallel version with different thread counts
for threads in 2 4 6 8 10 12; do
    echo "Running parallel iterations=$n threads=$threads"
    # Execute the program with the given iteration count and threads,
    # capture its comma‐separated output line, and append it to the CSV
    line=$($EXEC -i 5000 -t $threads)
    echo "$line" >> "$OUTFILE"
done


# Generate plots from the results CSV
python3 plot_results.py "$OUTFILE"

# Clean up build artifacts
make clean
