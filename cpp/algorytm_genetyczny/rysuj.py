import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.collections import PatchCollection
import numpy as np
import csv

fig = plt.figure()
ax = fig.add_subplot(111, aspect='equal')

plt.xlim(-1000,3800)
plt.ylim(-1000,3070)

ax.xaxis.tick_top()
plt.gca().invert_yaxis()

i=0
dane = []
for line in open("output.txt", "r"):
	line=line[:-1]
	if i==0:
		i+=1
		continue
	else:
		dane.append([int(l) for l in line.split(" ")])
	i+=1
	
ax.add_patch(patches.Rectangle((0,0),2800,2070,linewidth=3,edgecolor='blue',facecolor='none', fill=False))
#ax.add_patch(patches.Rectangle((0,0),4096,4096,linewidth=2,edgecolor='gray',facecolor='none', fill=False))

for d in dane:
	if d[2] != -1:
		if d[4]:
			ax.add_patch(patches.Rectangle((d[2],d[3]),d[1],d[0],linewidth=1,edgecolor='red',facecolor='none', fill=False))
		else:
			ax.add_patch(patches.Rectangle((d[2],d[3]),d[0],d[1],linewidth=1,edgecolor='red',facecolor='none', fill=False))
plt.savefig("wykres.png");
plt.show()
