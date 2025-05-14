#!/bin/bash

# Compile the program using Makefile
make

# Remove existing plots directory (clean old plots)
rm -rf plots

# Define the executable path
EXEC=./src/my_program

# Output CSV file for benchmarking results
OUTFILE="results.csv"
echo "n,threads,generations,time" > "$OUTFILE"  # CSV header

# Loop over different array sizes (n)
for n in 64 1024 4096; do
    # Serial execution with 1 thread
    echo "Running serial array_size=$n"
    line=$($EXEC 1000 $n 0 1)  # Run with is_parallel=0 and 1 thread
    echo "$line" >> "$OUTFILE"

    # Parallel execution with multiple thread counts
    for threads in 2 4 6 8; do
        echo "Running parallel grid_size=$n threads=$threads"
        line=$($EXEC 1000 $n 1 $threads)  # Run with is_parallel=1
        echo "$line" >> "$OUTFILE"
    done
done

# Generate plots from the results using Python script
python3 plot_results.py "$OUTFILE"

# Clean up compiled files
make clean
