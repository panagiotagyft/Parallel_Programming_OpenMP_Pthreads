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

# Διόρθωση τιμών για uniform labels
df['layout'] = df['layout'].str.strip().str.lower()
df['threads'] = df['threads'].astype(str)  # ώστε να περιλαμβάνει "serial" ως κείμενο

# Για κάθε διάταξη: row wise ή column wise
for layout in df['layout'].unique():
    subset = df[df['layout'] == layout]
    plt.figure()

    # Για κάθε διαφορετικό πλήθος νημάτων (ή "serial")
    for threads in sorted(subset['threads'].unique(), key=lambda x: int(x) if x.isdigit() else -1):
        group = subset[subset['threads'] == threads]
        plt.plot(group['n'], group['time'], marker='o', label=f"{threads} threads" if threads != "serial" else "serial")

    plt.title(f"{layout.capitalize()} performance")
    plt.xlabel("Matrix size (n)")
    plt.ylabel("Execution Time (s)")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"{output_dir}/{layout.replace(' ', '_')}_plot.png")
    plt.close()
