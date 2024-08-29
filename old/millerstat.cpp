/* for p primes from 0 to 1000 i want to take an element A from {-1, 1} at random and calculate p^2 + A p^{1.5} + p */

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "helperfunctions.h"

using namespace std;

int main( int argc, char *argv[] ) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_value>" << std::endl;
        return 1;
    }

    int input_value = std::stoi(argv[1]);


    vector<int> primes = generate_primes_in_range(0, input_value);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);

    ofstream output_file1("randomdatapos.txt");
    if (!output_file1.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    for (int p : primes) {
        int A = dis(gen) == 0 ? -1 : 1;
        //cout << A << endl;
        double result = A + 1 / pow(p, 0.5);
        output_file1 << p << "," << result << endl;
    }

    ofstream output_file2("randomdataneg.txt");
    if (!output_file1.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    for (int p : primes) {
        int A = dis(gen) == 0 ? -1 : 1;
        double result =  A - 1 / pow(p, 0.5);
        output_file2 << p << "," << result << endl;
    }

    return 0;
}