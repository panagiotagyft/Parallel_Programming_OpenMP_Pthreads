#!/bin/bash

make

rm -rf plots


EXEC=./src/my_program
OUTFILE="results.csv"

# Check if exercise 6 results exist
if [ ! -f "../exercise_6/results.csv" ]; then
    echo "Exercise 6 has not been executed yet. Running exercise 6 now..."

    # Change directory and run the exercise 6 executable
    cd ../exercise_6 || { echo "Cannot enter exercise_6 directory"; exit 1; }

    # Run the executable (adjust this to match your actual binary name and parameters)
    ./experiments.sh

    # Return to original directory
    cd - > /dev/null
fi

echo "Running exercise 9..."

echo "n,threads,generations,time" > "$OUTFILE"
echo


for n in 64 1024 4096; do
    
    echo "Running serial array_size=$n"
    line=$($EXEC -g 1000 -gs $n -impl 0 -t 1)
    echo "$line" >> "$OUTFILE"

    for threads in 2 4 6 8; do
        echo "Running parallel grid_size=$n threads=$threads"
        line=$($EXEC -g 1000 -gs $n -impl 1 -t $threads) 
        echo "$line" >> "$OUTFILE"
    done
done

python3 plot_results.py 

make clean