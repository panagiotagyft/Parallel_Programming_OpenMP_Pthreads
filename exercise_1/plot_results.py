import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Input
filename = sys.argv[1]
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# 1) Load the data
df = pd.read_csv(filename)

# 2) Split out the series we need
serial = df[df['Implementation'] == 'Serial']
openmp = df[df['Implementation'] == 'OpenMp']
pthreads = df[df['Implementation'] == 'Pthreads']

# Thread counts to plot
thread_counts = [1, 2, 4, 6, 8]

# 3) Plot Serial vs OpenMP
plt.figure()
for t in thread_counts:
    if t == 1:
        sub = serial
        label = 'Serial'
    else:
        sub = openmp[openmp['threads'] == t]
        label = f'{t} threads'
    plt.plot(sub['num_darts'], sub['time'], marker='o', label=label)

plt.xlabel('num_darts')
plt.ylabel('time (s)')
plt.title('Serial vs OpenMP')
plt.legend()
plt.grid(True)
plt.savefig(f'{output_dir}/serial_vs_openmp.png')
plt.close()

# 4) Plot Serial vs Pthreads
plt.figure()
for t in thread_counts:
    if t == 1:
        sub = serial
        label = 'Serial'
    else:
        sub = pthreads[pthreads['threads'] == t]
        label = f'{t} threads'
    plt.plot(sub['num_darts'], sub['time'], marker='o', label=label)

plt.xlabel('num_darts')
plt.ylabel('time (s)')
plt.title('Serial vs Pthreads')
plt.legend()
plt.grid(True)
plt.savefig(f'{output_dir}/serial_vs_pthreads.png')
plt.close()

