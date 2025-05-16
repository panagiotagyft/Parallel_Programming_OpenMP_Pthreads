import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

filename = sys.argv[1]
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

df = pd.read_csv(filename)
df['threads'] = df['threads'].astype(str) 

plt.figure()

for threads in sorted(df['threads'].unique(), key=lambda x: int(x) if x.isdigit() else -1):
    group = df[df['threads'] == threads]
    plt.plot(group['n'], group['time'], marker='o', label=f"{threads} threads" if threads != "serial" else "serial")

plt.title("Execution Time vs Matrix Size")
plt.xlabel("Matrix size (n)")
plt.ylabel("Execution Time (s)")
plt.legend()
plt.grid(True)
plt.savefig(f"{output_dir}/execution_time_plot.png")
plt.close()
