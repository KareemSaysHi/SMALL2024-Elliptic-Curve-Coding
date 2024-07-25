import csv
import itertools
import matplotlib.pyplot as plt

# Read data back from the file
x_pos = []
y_pos = []
x_neg = []
y_neg = []
sumofdata = 0
count = 0
with open('randomdatapos.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        count += 1
        sumofdata += float(data[1])
        x_pos.append(int(data[0]))
        y_pos.append(sumofdata / count)

sumofdata = 0
count = 0
with open('randomdataneg.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        count += 1
        sumofdata += float(data[1])
        x_neg.append(int(data[0]))
        y_neg.append(sumofdata / count)

#Sort the data
x_pos, y_pos = zip(*sorted(zip(x_pos, y_pos)))
x_neg, y_neg = zip(*sorted(zip(x_neg, y_neg)))

# Plot the new data
plt.plot(x_pos, y_pos, '-')
plt.plot(x_neg, y_neg, '-')

plt.axhline(y=0, color='r', linestyle='-')  # Adds a horizontal line at y=0 with red color and solid line style
plt.xlabel('Prime')
plt.ylabel('Running average of second moment at prime')
plt.legend(loc='best')
plt.show()


