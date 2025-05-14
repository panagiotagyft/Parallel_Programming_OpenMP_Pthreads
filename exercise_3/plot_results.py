import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Input: CSV file containing measurements
filename = sys.argv[1]
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Load the CSV into a DataFrame
df = pd.read_csv(filename)

# Group the data by the number of iterations
iteration_groups = df.groupby("iterations")

for iterations, group in iteration_groups:
    plt.figure()

    # Within each iteration group, plot each lock type
    for lock_type, lock_group in group.groupby("lock"):
        plt.plot(
            lock_group["threads"],    # number of threads on the x-axis
            lock_group["time"],       # execution time on the y-axis
            marker='o',
            label=lock_type
        )

    # Set chart title and axis labels
    plt.title(f"Execution Time vs Threads (Iterations = {iterations})")
    plt.xlabel("Threads")
    plt.ylabel("Time (seconds)")
    plt.legend()
    plt.grid(True)

    # Save the plot to a file named by the iteration count
    plt.savefig(f"{output_dir}/plot_iterations_{iterations}.png")
    plt.close()
