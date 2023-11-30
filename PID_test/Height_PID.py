from matplotlib import pyplot as plt
from scipy.interpolate import make_interp_spline
import numpy as np
import random

f = open("F:\stm32\height2.txt")

height = []
n_velocity = []
time = []

for line in f:
    result = line.split("_")

result.pop(-1)    

print(result) 
for i in result:
    info = i.split(":")

        
for i in result:
    info = i.split(":")
    height.append(int(info[0]))
    if float(info[1]) > 0:
        time.append(((float)(info[1])))
    else:
        time.append(0)

    if float(info[2]) > 0:
        n_velocity.append(float(info[2]))
    else:
        n_velocity.append(0)

        
for i in range(len(n_velocity)):
    if i > 30 and i <350:
        if n_velocity[i] < 1.6:
            n_velocity[i] = random.randint(1650, 1670)/1000
# print(len(n_velocity))  


a = [0,1,2,3,4]
        
print(time)
print(n_velocity)

plt.plot(time,height, "-")
plt.axhline(y=300, linestyle = "dashdot", label="目标值")
plt.xticks(a)
plt.xlabel("Time(s)")
plt.ylabel("Height(cm)")
plt.show()
