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

acceleration = []
for i in range(len(seq)):
    acceleration.append(seq[i] / cuda[i])


plt.title("Problema - Análise Estatística")
plt.ylabel("Acaleração")
plt.xticks(labels)
plt.plot(labels, acceleration)
plt.plot(labels, [1] * len(labels), "--", color="black", alpha=0.35)
# plt.legend()
plt.savefig("chart_acc.png")