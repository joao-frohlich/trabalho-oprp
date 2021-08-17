import pandas as pd
from scipy.stats.mstats import gmean
import sys

filename = sys.argv[1]
with open("entradas/" + filename) as f:
    data = f.read()
    v = list(map(float,data.split()))

v = pd.Series(
    v
)
ma = v.mean()
dp = v.std()
mg = gmean(v)
md = v.median()
p95 = v.quantile(0.95)
mini = v.min()
maxi = v.max()
print("{} {:.6f} {:.6f} {:.6f} {:.6f} {:.6f} {:.6f} {:.6f}".format(filename, ma, dp, mg, md, p95, mini, maxi))