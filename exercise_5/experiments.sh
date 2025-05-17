#!/bin/bash

make

rm -rf plots

EXEC=./src/my_program
OUTFILE="results.csv"

echo "Threads,Type,Schedule,Chunk,Time" > "$OUTFILE"

threads_list=(2 4 6 8)
schedules=("Static" "Dynamic" "Guided")
chunks=(1 4 7)

for threads in "${threads_list[@]}"; do
    for sched in "${schedules[@]}"; do
        for chunk in "${chunks[@]}"; do
            export OMP_SCHEDULE="$sched,$chunk"
            echo -n "$threads,Schedule,$sched,$chunk," >> "$OUTFILE"
            $EXEC $threads 10000 10000 >> "$OUTFILE"
        done
    done

    # full matrix
    echo -n "$threads,Baseline,Full,," >> "$OUTFILE"
    $EXEC $threads 10000 10000 full >> "$OUTFILE"

    # upper triangular
    echo -n "$threads,Baseline,Upper,," >> "$OUTFILE"
    $EXEC $threads 10000 10000 upper >> "$OUTFILE"
done

# Generate plots from the results CSV
python3 plot_results.py "$OUTFILE"

# Clean up build artifacts
make clean
