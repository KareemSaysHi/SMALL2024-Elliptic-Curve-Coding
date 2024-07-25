import csv
import itertools
import matplotlib.pyplot as plt

# Read data back from the file
x_new = []
y_new = []
sumofdata = 0
count = 0
with open('randomdataneg.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        count += 1
        sumofdata += float(data[1])
        x_new.append(int(data[0]))
        y_new.append(sumofdata / count)

#Sort the data
x_new, y_new = zip(*sorted(zip(x_new, y_new)))

# Plot the new data
plt.plot(x_new, y_new, '-')
plt.axhline(y=0, color='r', linestyle='-')  # Adds a horizontal line at y=0 with red color and solid line style
plt.xlabel('Prime')
plt.ylabel('Running average of second moment at prime')
plt.title('Running average of second moment of x^3 + x + \sum_{i=0}^{500} T^i, bad primes thrown out')
plt.show()


