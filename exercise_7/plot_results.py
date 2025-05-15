import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Input: CSV file with performance measurements
filename = sys.argv[1]
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Load the CSV file
df = pd.read_csv(filename)

# Normalize values for consistent labeling
df['row_or_col'] = df['row_or_col'].str.strip().str.lower()
df['threads'] = df['threads'].astype(str)  # Convert to string to handle "serial" as text

# For each layout type: row-wise or column-wise
for row_or_col in df['row_or_col'].unique():
    subset = df[df['row_or_col'] == row_or_col]
    plt.figure()

    # For each unique thread count (or "serial")
    for threads in sorted(subset['threads'].unique(), key=lambda x: int(x) if x.isdigit() else -1):
        group = subset[subset['threads'] == threads]
        plt.plot(group['n'], group['time'], marker='o', label=f"{threads} threads" if threads != "serial" else "serial")

    plt.title(f"{row_or_col.capitalize()} performance")
    plt.xlabel("Matrix size (n)")
    plt.ylabel("Execution Time (s)")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"{output_dir}/{row_or_col.replace(' ', '_')}_plot.png")
    plt.close()
