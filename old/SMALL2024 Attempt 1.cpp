#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

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
    } else if (power_mod_p(x, int((p-1)/2), p) == 1) {
        return 1;
    } else {
        return -1;
    }
}

int testLegendreStuff(int p) {
    int result = 0;
    for (int x = 0; x < p; x++) {
        result = legendre_symbol(x, p);
        cout << "Testing " << x << "/" << p << ", it is " << result << "\n";
    }
    return 0;
}

// Function to generate prime numbers up to k
vector<int> generate_primes_in_range(int lower, int upper) {
    vector<int> primes;
    for (int num = 2; num < upper; num++) {
        bool is_prime = true;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime && (num >= lower)) {
            primes.push_back(num);
        }
    }
    return primes;
}

void computer_a_constants(int lower, int upper) {
    vector<int> primes = generate_primes_in_range(lower, upper);

    // Create a 2D array to store Legendre symbols
    vector<vector<int>> bigArray(n, vector<int>(n, 0));

    // Function to calculate Legendre symbols for each prime
    for (int p : primes) {
        for (int x = 0; x < p; ++x) {
            bigArray[p][x] = legendre_symbol(x, p);
        }
    }

    // Write results to file
    ofstream file("legendreResults.txt");
    if (file.is_open()) {
        for (int prime : primes) {
            cout << "At prime " << prime << endl;
            for (int a = 0; a < prime; a++) {
                for (int b = 0; b < prime; b++) {
                    long result = 0;
                    for (int x = 0; x < prime; x++) {
                        int value = (power_mod_p((long)x, 3, prime) + a * x + b) % prime;
                        //cout << "p = " << prime << ", a = " << a << ", b = " << b << ", x = " << x << ", value = " << value << "\n";
                        //cout << "value = " << bigArray[prime][value] << "\n";
                    
                        result += bigArray[prime][value];
                    }
                    file << prime << ", " << a << ", " << b << ": " << result << "\n";
                }
            }
            file << "\n\n";
        }

        file.close();
    } else {
        cerr << "Unable to open file";
    }

}

// Main function
int main() {

    iif (argc != 3) {
        cerr << "Usage: " << argv[0] << " <file_name> <upper_limit>" << endl;
        return 1;
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);

    compute_a_constants(int lower, int upper);
    
    return 0;

    // generate primes up to n
    
}