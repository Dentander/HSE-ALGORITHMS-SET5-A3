import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data/new_g1.2.csv")

t = df["t"]
mean = df["mean_estimate"]
real_count = df["real_count"]
sigma = df["sigma"]

plt.figure()
plt.plot(t, mean, label=r"$\mathbb{E}(N_t)$")
plt.fill_between(
    t,
    mean - sigma,
    mean + sigma,
    alpha=0.3,
    label=r"$\pm \sigma_{N_t}$",
    color='red'
)

plt.xlabel("t")
plt.ylabel("Number of unique elements")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# =======================
plt.plot(t, sigma * sigma, label=r"Полученная дисперсия")
plt.plot(t, (real_count * 0.00288)**2, label=r"Ожидаемая дисперсия")

plt.xlabel("t")
plt.ylabel("Metrics")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()


