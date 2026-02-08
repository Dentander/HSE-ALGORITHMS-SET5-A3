import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data/new_g1.1.csv")

print(df.head())

plt.figure()
plt.plot(df["t"], df["real_count"], label=r"$F_0^t$")
plt.plot(df["t"], df["hll_estimate"], label=r"$N_t$")
plt.xlabel("t")
plt.ylabel("Number of unique elements")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
