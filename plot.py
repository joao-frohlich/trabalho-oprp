import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import math

plt.style.use("ggplot")

seq, cuda = [], []
labels = range(1, 19)

with open("seq_time") as f:
    for line in f:
        seq.append(float(line.replace("s", "")))

with open("cuda_time") as f:
    for line in f:
        cuda.append(float(line.replace("s", "")))

x = np.arange(len(labels))
width = 0.35

fig, ax = plt.subplots()
rects1 = ax.bar(x - width / 2, seq, width, align="center", label="Sequencial")
rects2 = ax.bar(x + width / 2, cuda, width, align="center", label="OpenMP + CUDA")

ax.set_ylabel("Tempo (s)")
ax.set_title("Problema - Análise Estatística")
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()
plt.savefig("chart_time.png")