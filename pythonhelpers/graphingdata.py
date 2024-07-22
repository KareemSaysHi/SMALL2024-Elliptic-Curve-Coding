import csv
import itertools
import matplotlib.pyplot as plt

# Read data back from the file
x_new = []
y_new = []
with open('data.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        x_new.append(int(data[0]))
        y_new.append(float(data[1]))

# Plot the new data
plt.plot(x_new, y_new, '.')
plt.xlabel('Prime')
plt.ylabel('Second moment')
plt.title('Second moment for some family')
plt.show()


