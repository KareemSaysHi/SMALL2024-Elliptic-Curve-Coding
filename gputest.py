import math
import numpy as np
from numba import cuda, jit
import csv
import sys

@jit(nopython=True)
def power_mod_p(a, b, p):
    result = 1
    while b > 0:
        if b % 2 == 1:
            result = (result * a) % p
        b = b >> 1
        a = (a * a) % p
    return result % p

@jit(nopython=True)
def legendre_symbol(x, p):
    if x % p == 0:
        return 0
    elif power_mod_p(x, (p-1)//2, p) == 1:
        return 1
    else:
        return -1

def generate_primes_in_range(lower, upper):
    primes = []
    for num in range(2, upper):
        is_prime = True
        for i in range(2, int(math.sqrt(num)) + 1):
            if num % i == 0:
                is_prime = False
                break
        if is_prime and num >= lower:
            primes.append(num)
    return primes

@cuda.jit
def compute_legendre_symbols(primes, lower, upper, big_array):
    p_idx = cuda.grid(1)
    if p_idx >= primes.size:
        return
    
    p = primes[p_idx]
    for x in range(p):
        big_array[p - lower, x] = legendre_symbol(x, p)

@cuda.jit
def compute_results(primes, lower, big_array, results):
    p_idx, a_idx, b_idx = cuda.grid(3)
    if p_idx >= primes.size:
        return
    
    p = primes[p_idx]
    a = a_idx
    b = b_idx
    if a >= p or b >= p:
        return

    result = 0
    for x in range(p):
        value = (power_mod_p(x, 3, p) + a * x + b) % p
        result += big_array[p - lower, value]
    results[p_idx, a, b] = result

def compute_a_constants(lower, upper):
    primes = np.array(generate_primes_in_range(lower, upper), dtype=np.int32)
    num_primes = primes.size

    big_array = np.zeros((upper - lower, upper), dtype=np.int32)
    threads_per_block = 256
    blocks_per_grid = (num_primes + (threads_per_block - 1)) // threads_per_block
    compute_legendre_symbols[blocks_per_grid, threads_per_block](primes, lower, upper, big_array)

    results = np.zeros((num_primes, upper, upper), dtype=np.int64)
    threads_per_block_3d = (8, 8, 8)
    blocks_per_grid_3d = (
        (num_primes + threads_per_block_3d[0] - 1) // threads_per_block_3d[0],
        (upper + threads_per_block_3d[1] - 1) // threads_per_block_3d[1],
        (upper + threads_per_block_3d[2] - 1) // threads_per_block_3d[2],
    )
    compute_results[blocks_per_grid_3d, threads_per_block_3d](primes, lower, big_array, results)

    for p_idx in range(num_primes):
        prime = primes[p_idx]
        filename = f"newdatapython/file_{prime}.csv"
        with open(filename, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(["prime", "a", "b", "value"])
            for a in range(prime):
                for b in range(prime):
                    writer.writerow([prime, a, b, results[p_idx, a, b]])

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <file_name> <upper_limit>")
        return 1

    lower = int(sys.argv[1])
    upper = int(sys.argv[2])

    compute_a_constants(lower, upper)

if __name__ == "__main__":
    main()