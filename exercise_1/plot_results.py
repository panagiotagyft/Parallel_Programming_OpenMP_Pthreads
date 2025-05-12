import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

# 1) Φόρτωση CSV & καθάρισμα headers
df = pd.read_csv(sys.argv[1], skipinitialspace=True)
df.columns = df.columns.str.strip()

# 2) Φάκελος για τα outputs
os.makedirs("plots", exist_ok=True)

# 3) Πάρε τα δεδομένα για την "Serial"
serial = df[df["Implementation"] == "Serial"].sort_values("num_darts")

# 4) Για κάθε μία από τις δύο υλοποιήσεις που θέλεις να συγκρίνεις:
for impl in ["OpenMp", "Pthreads"]:
    # φιλτράρω μόνο τις γραμμές της συγκεκριμένης υλοποίησης
    sub_imp = df[df["Implementation"] == impl]
    # βρίσκω μοναδικούς αριθμούς threads (π.χ. [2,4,6,8])
    threads = sorted(sub_imp["threads"].unique())

    plt.figure(figsize=(8,5))

    # α) πάντα σχεδιάζω πρώτα τη Serial
    plt.plot(
        serial["num_darts"],
        serial["time"],
        marker="o",
        linestyle="-",
        label="Serial"
    )

    # β) μετά κάθε σειρά για την impl, σε κάθε threads
    for t in threads:
        sub_t = sub_imp[sub_imp["threads"] == t].sort_values("num_darts")
        plt.plot(
            sub_t["num_darts"],
            sub_t["time"],
            marker="o",
            linestyle="--",
            label=f"{impl} ({t} threads)"
        )

    # 5) μορφοποίηση
    plt.title(f"Serial vs {impl}  – Time vs Number of Darts")
    plt.xlabel("Number of Darts")
    plt.ylabel("Time (sec)")
    plt.xscale("log")                 # λογαριθμική κλίμακα στον άξονα x
    plt.grid(True, which="both", ls=":")
    plt.legend()
    plt.tight_layout()

    # 6) αποθήκευση
    out = impl.lower()
    plt.savefig(f"plots/serial_vs_{out}.png", dpi=300)
    plt.close()
