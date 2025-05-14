import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Get the input filename from command-line arguments
filename = sys.argv[1]

# Define the output directory for plots
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)  # Create the directory if it doesn't exist

# Load the CSV data
df = pd.read_csv(filename)

# Convert the 'threads' column to string to allow labels like "serial"
df['threads'] = df['threads'].astype(str)

plt.figure()

# Group the data by thread count (or "serial") and plot each group
for threads in sorted(df['threads'].unique(), key=lambda x: int(x) if x.isdigit() else -1):
    group = df[df['threads'] == threads]
    label = f"{threads} threads" if threads != "serial" else "serial"
    plt.plot(group['n'], group['time'], marker='o', label=label)

# Add plot title and axis labels
plt.title("Execution Time vs Grid Size for 1000 generations")
plt.xlabel("Grid size (n)")
plt.ylabel("Execution Time (s)")

# Show legend and grid
plt.legend()
plt.grid(True)

# Save the figure as a PNG file
plt.savefig(f"{output_dir}/execution_time_plot.png")
plt.close()
