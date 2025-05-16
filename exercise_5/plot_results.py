import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

# Input: CSV file containing measurements
filename = sys.argv[1]

# Directory to save generated plots
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Load data from CSV into a DataFrame
# The CSV is expected to have columns: iterations, barrier (lock type), threads, time
df = pd.read_csv(filename)

df['Threads'] = df['Threads'].astype(int)
df['Time'] = df['Time'].astype(float)


for sched in ["Static", "Dynamic", "Guided"]:
    plt.figure(figsize=(8, 5))
    sched_df = df[(df["Type"] == "Schedule") & (df["Schedule"] == sched)]

    for chunk in sorted(sched_df["Chunk"].dropna().unique()):
        chunk_df = sched_df[sched_df["Chunk"] == chunk]
        plt.plot(chunk_df["Threads"], chunk_df["Time"], marker="o", label=f"chunk = {int(chunk)}")

    # Baselines
    base_full = df[(df["Type"] == "Baseline") & (df["Schedule"] == "Full")]
    base_upper = df[(df["Type"] == "Baseline") & (df["Schedule"] == "Upper")]
    plt.plot(base_full["Threads"], base_full["Time"], linestyle='--', color='gray', label="Baseline Full")
    plt.plot(base_upper["Threads"], base_upper["Time"], linestyle='--', color='black', label="Baseline Upper")

    plt.title(f"Schedule: {sched}")
    plt.xlabel("Threads")
    plt.ylabel("Elapsed Time (seconds)")
    plt.legend()
    plt.grid(True)
    plot_filename = f"{output_dir}/plot_iterations_{sched}.png"
    plt.savefig(plot_filename)
    plt.close()
