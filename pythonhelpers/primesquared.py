import matplotlib.pyplot as plt


def is_prime(num):
    if num < 2:
        return False
    for i in range(2, int(num ** 0.5) + 1):
        if num % i == 0:
            return False
    return True

def sum_of_prime_squares(n):
    prime_squares_sum = 0
    for num in range(2, n + 1):
        if is_prime(num):
            prime_squares_sum += num ** 2
    return prime_squares_sum


x = []
y = []

print(sum_of_prime_squares(10000))


for n in range(2, 10001):
    x.append(n)
    result = sum_of_prime_squares(n)
    y.append(result)


plt.plot(x, y)
plt.xlabel('n')
plt.ylabel('Sum of Prime Squares')
plt.title('Sum of Prime Squares up to n')
plt.show()