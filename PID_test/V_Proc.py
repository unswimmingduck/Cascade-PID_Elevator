from matplotlib import pyplot as plt
from scipy.interpolate import make_interp_spline
import numpy as np
import random

f = open("F:\stm32\V_PID.txt")

V = []
T = []

for i in f:
    reslut = i.split('_')
for i in reslut:
    if("V" in i):
        V.append(float(i.split(":")[1]))
    elif("T" in i):
        T.append(float((i.split(":")[1]))/1000)

V = np.array(V)
T = np.array(T)

a = list( i/10 for i in range(20))


plt.plot(T,V, "-")
plt.xticks(a)
plt.axhline(y=1.6667, linestyle = "dashdot", label="目标值")
plt.xlabel("Time(s)")
plt.ylabel("rotation speed(rad/s)")
plt.show()

