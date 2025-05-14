import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Input: CSV file containing measurements
filename = sys.argv[1]

# Directory to save generated plots
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Load data from CSV into a DataFrame
# The CSV is expected to have columns: iterations, barrier (lock type), threads, time
df = pd.read_csv(filename)

# Group data by the number of iterations
iteration_groups = df.groupby("iterations")

# Loop through each iterations group and create a separate plot
for iterations, group in iteration_groups:
    plt.figure()

    # Within this iterations group, group again by lock (barrier) type
    for lock_type, lock_group in group.groupby("barrier"):
        # Plot execution time versus number of threads for each lock type
        plt.plot(
            lock_group["threads"],  # X-axis: number of threads
            lock_group["time"],     # Y-axis: execution time in seconds
            marker='o',               # Use circle markers for data points
            label=lock_type          # Legend label: lock type
        )

    # Set plot title and axis labels
    plt.title(f"Execution Time vs Threads (Iterations = {iterations})")
    plt.xlabel("Threads")
    plt.ylabel("Time (seconds)")

    # Display legend and grid for better readability
    plt.legend()
    plt.grid(True)

    # Save the plot as a PNG file, named according to the iterations value
    plot_filename = f"{output_dir}/plot_iterations_{iterations}.png"
    plt.savefig(plot_filename)
    plt.close()
