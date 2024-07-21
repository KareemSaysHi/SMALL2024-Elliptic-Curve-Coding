#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

// Function to check if a number is prime
bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i <= std::sqrt(num); ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

// Function to get all primes up to n
std::vector<int> get_primes_up_to_n(int n) {
    std::vector<int> primes;
    for (int num = 2; num < n; ++num) {
        if (is_prime(num)) {
            primes.push_back(num);
        }
    }
    return primes;
}

// Function to get value from a CSV file
int get_value_from_file(int prime, int a, int b, const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return -1;
    }

    std::string line;
    int target_line = prime * a + b + 1;
    int current_line = 0;
    
    while (std::getline(file, line)) {
        if (current_line == target_line) {
            std::stringstream ss(line);
            std::string item;
            int value;
            for (int i = 0; std::getline(ss, item, ','); ++i) {
                if (i == 3) {
                    value = std::stoi(item);
                    return value;
                }
            }
        }
        ++current_line;
    }
    return -1;
}

int main() {
    int n = 3000;
    std::vector<int> primes = get_primes_up_to_n(n);
    std::vector<double> A_list;

    for (int prime : primes) {
        int A = 0;
        std::cout << "on prime " << prime << std::endl;

        std::string filename = "newdata/file_" + std::to_string(prime) + ".csv";

        for (int t = 0; t < prime; ++t) {
            int t_cubed = (((t * t) % prime) * t) % prime;

            int value = get_value_from_file(prime, 1, t_cubed, filename);
            if (value != -1) {
                A += value * value;
            }
        }

        A_list.push_back((A - std::pow(prime, 2)) / std::pow(prime, 1.5));
    }

    std::vector<int> x;
    std::vector<double> y;

    std::vector<int> maskarray;
    for (int prime : primes) {
        if ((prime % 3 != 1) && prime >= 5) {
            maskarray.push_back(1);
        } else {
            maskarray.push_back(0);
        }
    }

    for (size_t i = 0; i < primes.size(); ++i) {
        if (maskarray[i]) {
            x.push_back(primes[i]);
            y.push_back(A_list[i]);
        }
    }

    std::ofstream output_file("data.txt");
    if (!output_file.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        output_file << x[i] << "," << y[i] << std::endl;
    }

    return 0;
}