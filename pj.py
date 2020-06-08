import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import numpy as np
import pandas as pd
from matplotlib import patches

lines = pd.read_csv('Q1_line.csv')
arcs = pd.read_csv('Q1_arc.csv')  
print(lines) 
print(arcs) 

plt.plot([lines.start_x,lines.end_x],[lines.start_y,lines.end_y])

plt.show()
