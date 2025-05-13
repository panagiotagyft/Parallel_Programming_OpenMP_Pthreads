import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Read the input CSV filename from the command-line arguments
filename = sys.argv[1]

# Define and create (if needed) an output directory for the plot images
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Load the CSV data into a pandas DataFrame
df = pd.read_csv(filename)

# Group the data by the number of iterations
iteration_groups = df.groupby("iterations")

# For each distinct iteration count, produce a separate plot
for iterations, group in iteration_groups:
    plt.figure()

    # Within that iteration group, further group by lock type
    for lock_type, lock_group in group.groupby("lock"):
        # Plot threads vs. execution time for this lock type
        plt.plot(
            lock_group["threads"],  # x-axis: number of threads
            lock_group["time"],     # y-axis: execution time in seconds
            marker='o',             # use circle markers at each data point
            label=lock_type         # label this line with the lock type
        )

    # Add title and axis labels
    plt.title(f"Execution Time vs Threads (Iterations = {iterations})")
    plt.xlabel("Threads")
    plt.ylabel("Time (seconds)")

    # Show legend and enable grid lines for readability
    plt.legend()
    plt.grid(True)

    # Save the current figure to a PNG file named by iteration count
    plt.savefig(f"{output_dir}/plot_iterations_{iterations}.png")

    # Close the figure to free memory before the next iteration
    plt.close()
