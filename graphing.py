import matplotlib.pyplot as plt
import math

plt.rcParams.update({
    "text.usetex": True,
    "font.family": "Helvetica"
})

# Read data back from the file
x_new = [] # primes
y_new = [] # normalized second moment
z_new = [] # unweighted running average
w_new = [] # weighted running average

title = input("What do you want the title to be? ")

count = 0
with open('data.csv', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        x_new.append(int(data[0]))
        y_new.append(float(data[1]))

# sort the data
x_new, y_new = zip(*sorted(zip(x_new, y_new)))

# perform averaging and weighted averaging
sumofdata = 0 #unweighted sum
wsumofdata = 0 #log weighted sum
weightedcount = 0
for i in range(0, len(y_new)):
    sumofdata += y_new[i]
    wsumofdata += (y_new[i] * math.log(x_new[i]))
    weightedcount += math.log(x_new[i])
    z_new.append(sumofdata/float(i+1))
    w_new.append(wsumofdata/float(weightedcount))

# Plot the data
fig, axs = plt.subplots(3)
axs[0].plot(x_new, y_new, '.')
axs[1].plot(x_new, w_new, '-', color = "blue", label="unweighted")
axs[1].plot(x_new, z_new, '-', color = "red", label="$\log p$ weighted")
axs[2].plot(x_new, w_new, '-', color = "blue", label="unweighted")
axs[2].plot(x_new, z_new, '-', color = "red", label="$\log p$ weighted")
# plt.plot(x_new, w_new, '-', color = "red", label = "Weighted")
# plt.plot(x_new, w_new, '-', color = "red", label = "Weighted")
axs[0].set(xlabel='Prime', ylabel='Normalized second moment at prime')
axs[1].set(xlabel='Prime', ylabel='Running average at prime')
axs[2].set(xlabel='Prime', ylabel='Running average at prime')
axs[2].set_ylim(-.1,.1)
# axs[2].set(xlabel='Prime', ylabel='Log Weighted Running average of second moment at prime')
axs[0].set_title('Normalized second moment of ' + title)
axs[1].set_title('Running averages of the normalized second moment of ' + title)
axs[2].set_title('Running averages of the normalized second moment of ' + title)
axs[1].legend()
#axs[2].set_title('Log weighted and unweighted running average of ' + title)
#axs[2].legend()
# axs[2].set_title('Log weighted running average of ' + title)
axs[0].axhline(y = 0, color = 'black')
axs[1].axhline(y = 0, color = 'black')
axs[2].axhline(y = 0, color = 'black')
#plt.ylim(-.1, .1)
#plt.title('Running average of normalized second moment of ' + title)
#plt.legend(loc="upper right")
plt.subplots_adjust(hspace = .5)
plt.show()