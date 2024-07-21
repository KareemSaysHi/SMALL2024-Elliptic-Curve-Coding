#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

mutex file_mutex;

long power_mod_p(long a, int b, int p) {
    long result = 1;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result * a) % p;
        }
        b = b >> 1;
        a = (a * a) % p;
    }
    return result % p;
}

// Function to calculate Legendre symbol
int legendre_symbol(int x, int p) {
    if (x % p == 0) {
        return 0;
    } else if (power_mod_p(x, (p - 1) / 2, p) == 1) {
        return 1;
    } else {
        return -1;
    }
}

// Function to generate prime numbers up to k
vector<int> generate_primes(int k) {
    vector<int> primes;
    vector<thread> threads;
    mutex primes_mutex;

    auto check_prime = [&](int num) {
        bool is_prime = true;
        for (int i = 2; i <= sqrt(num); ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            lock_guard<mutex> guard(primes_mutex);
            primes.push_back(num);
        }
    };

    for (int num = 2; num < k; ++num) {
        threads.push_back(thread(check_prime, num));
    }

    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return primes;
}

// Function to compute and write Legendre symbols to file
void compute_legendre_symbols(int prime, const vector<vector<int>> &bigArray) {
    ofstream file;
    {
        lock_guard<mutex> guard(file_mutex);
        file.open("legendreResults.txt", ios_base::app);
    }
    
    if (file.is_open()) {
        for (int a = 0; a < prime; a++) {
            for (int b = 0; b < prime; b++) {
                long result = 0;
                for (int x = 0; x < prime; x++) {
                    int value = (power_mod_p((long)x, 3, prime) + a * x + b) % prime;
                    result += bigArray[prime][value];
                }
                file << prime << ", " << a << ", " << b << ": " << result << "\n";
            }
        }
        file << "\n\n";
        file.close();
    } else {
        cerr << "Unable to open file";
    }
}

// Main function
int main() {
    const int n = 500;

    // Generate primes up to n
    vector<int> primes = generate_primes(n);

    // Create a 2D array to store Legendre symbols
    vector<vector<int>> bigArray(n, vector<int>(n, 0));
    vector<thread> threads;

    // Compute Legendre symbols for each prime in parallel
    for (int p : primes) {
        threads.push_back(thread([&, p] {
            for (int x = 0; x < p; ++x) {
                bigArray[p][x] = legendre_symbol(x, p);
            }
        }));
    }

    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    threads.clear();

    // Compute Legendre symbols and write to file in parallel
    for (int p : primes) {
        threads.push_back(thread(compute_legendre_symbols, p, cref(bigArray)));
    }

    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}