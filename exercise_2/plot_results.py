import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Είσοδος: αρχείο CSV με μετρήσεις
filename = sys.argv[1]
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Φόρτωση CSV
df = pd.read_csv(filename)

# Ομαδοποίηση κατά iterations
iteration_groups = df.groupby("iterations")

for iterations, group in iteration_groups:
    plt.figure()

    # Ομαδοποίηση κατά lock τύπο
    for lock_type, lock_group in group.groupby("lock"):
        plt.plot(
            lock_group["threads"],
            lock_group["time"],
            marker='o',
            label=lock_type
        )

    plt.title(f"Execution Time vs Threads (Iterations = {iterations})")
    plt.xlabel("Threads")
    plt.ylabel("Time (seconds)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"{output_dir}/plot_iterations_{iterations}.png")
    plt.show()
