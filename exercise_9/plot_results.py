import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Load the two CSV files
df1 = pd.read_csv("../exercise_6/results.csv")  # π.χ. το φιλτραρισμένο παράλληλο
df2 = pd.read_csv("results.csv")    # π.χ. μόνο σειριακό

output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Standardize column names
df1.columns = df2.columns = ["n", "threads", "generations", "time"]

# Keep only numeric threads
df1 = df1[df1["threads"].apply(lambda x: str(x).isdigit())]
df2 = df2[df2["threads"].apply(lambda x: str(x).isdigit())]

df1["threads"] = df1["threads"].astype(int)
df2["threads"] = df2["threads"].astype(int)

# Merge the two datasets on (n, threads, generations)
merged = pd.merge(df1, df2, on=["n", "threads", "generations"], suffixes=('_colapse(2)', '_task'))

# Plot one figure per thread count
for thread in sorted(merged['threads'].unique()):
    thread_data = merged[merged['threads'] == thread]
    
    plt.figure()
    plt.plot(thread_data['n'], thread_data['time_colapse(2)'], label='colapse(2)', marker='o')
    plt.plot(thread_data['n'], thread_data['time_task'], label='task', marker='x')
    plt.xlabel("n (grid size)")
    plt.ylabel("Execution Time (s)")
    plt.title(f"Performance Comparison - {thread} Threads")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"{output_dir}/execution_time_plot_{thread}.png")
    plt.close()