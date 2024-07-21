# 1st step: choose polynomial X^3 + X + T^3
# 2nd step: for each p:
    # for T mod p
        # compute T^3
        # lookup
    #square it
#sum it up
#subtract p^2, divide by p^3/2
#put it as one point in the graph

import csv
import itertools
import matplotlib.pyplot as plt
import math

'''

def is_prime(num):
    if num < 2:
        return False
    for i in range(2, int(num ** 0.5) + 1):
        if num % i == 0:
            return False
    return True

def get_primes_up_to_n(n):
    primes = []
    num = 2
    while num < n:
        if is_prime(num):
            primes.append(num)
        num += 1
    return primes

def get_value_from_file(prime, a, b, filename):
    with open(filename, 'r') as file:
        row = next(csv.reader(itertools.islice(file, prime * a + b + 1, prime * a + b + 2)))
        #print(str(row))
        return int(row[3])

n = 1000
primes = get_primes_up_to_n(n)
A_list = []

for prime in primes:
    A = 0
    print("on prime " + str(prime))

    filename = "newdata/file_" + str(prime) + ".csv" #open the file
    

    for t in range (0, prime):
        t_cubed = (((t * t) % prime) * t) % prime #compute mod p
        #print("t_cubed is " + str(t_cubed))
            #prime, a, b: a*prime + b + 1
            
        A += get_value_from_file(prime, 1, t_cubed, filename)**2
    
    A_list.append((A - prime**2) / (prime**(1.5)))


x = primes
y = A_list

maskarray = []
for prime in primes:
    if (prime % 3 != 1) and prime >= 5:
        maskarray.append(1)
    else:
        maskarray.append(0)

x = list(itertools.compress(x, maskarray))
y = list(itertools.compress(y, maskarray))

# Write data to a file
with open('data.txt', 'w') as file:
    for i in range(len(x)):
        file.write(f'{x[i]},{y[i]}\n')
'''

# Read data back from the file
x_new = []
y_new = []
z_new = []
with open('data.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        x_new.append(int(data[0]))
        y_new.append(float(data[1]))
        z_new.append(-3/(math.sqrt(int(data[0]))))

        #z_new.append(-3/(math.sqrt(int(data[0]))) - 1/((math.sqrt(int(data[0]))**3)))

# Plot the new data
plt.plot(x_new, y_new, '.')
plt.plot(x_new, z_new, '.')
plt.xlabel('Prime')
plt.ylabel('Second moment')
plt.title('Second moments of elliptic curve family X^3 + X + T^3 for primes 2 mod 3')
plt.show()


