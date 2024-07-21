import csv
import itertools
import matplotlib.pyplot as plt
import math

# Read data back from the file
x_new = []
y_new = []
sumofdata = 0
count = 0
with open('datat3.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        count += 1
        sumofdata += float(data[1]) * math.log(int(data[0]))
        x_new.append(int(data[0]))
        y_new.append(sumofdata / int(data[0]))

# Plot the new data
plt.plot(x_new, y_new, '.')
plt.xlabel('Prime')
plt.ylabel('Running average of second moment at prime')
plt.title('Running average of second moment')
plt.show()


