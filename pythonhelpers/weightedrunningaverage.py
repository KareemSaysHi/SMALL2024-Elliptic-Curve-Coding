import csv
import itertools
import matplotlib.pyplot as plt
import math

# Read data back from the file
x_new = []
y_new = []
sumofdata = 0
count = 0
with open('data.csv', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        count += 1
        if count > 100:
            sumofdata += float(data[1]) * math.log(int(data[0]))

            x_new.append(int(data[0]))
            y_new.append(sumofdata / int(data[0]))

# sort the data
x_new, y_new = zip(*sorted(zip(x_new, y_new)))

# Plot the new data
plt.plot(x_new, y_new, '-')
plt.axhline(y=0, color='r', linestyle='-')  # Adds a horizontal line at y=0 with red color and solid line style
plt.xlabel('Prime')
plt.ylabel('Running average of second moment at prime')
plt.title('Weighted Running average of second moment')
plt.show()


