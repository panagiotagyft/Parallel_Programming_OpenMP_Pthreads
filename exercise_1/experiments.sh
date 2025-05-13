#!/bin/bash

# Compile the project
make

# Remove any existing plots directory
rm -rf plots

# Path to the executable
EXEC=./src/my_program

# Output CSV file for results
OUTFILE="results.csv"

# Write CSV header
echo "Implementation,threads,num_darts,arrows,time,pi" > "$OUTFILE"
echo

# Loop over number of darts from 100 million to 1 billion, in steps of 100 million
for ((n=100000000; n<=1000000000; n+=100000000)); do

    # For each dart count, run with different thread counts
    for threads in 1 2 4 6 8; do
        echo "Running darts=$n threads=$threads"
        # Execute the program and capture its output line
        line=$($EXEC -d $n -t $threads)
        # Append the output to the CSV file
        echo "$line" >> "$OUTFILE"
    done
done

# Plot the results using the Python script
python3 plot_results.py "$OUTFILE"

# Clean build artifacts
make clean
