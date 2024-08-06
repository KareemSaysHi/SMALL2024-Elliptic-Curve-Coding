#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h>
#include <cassert>
#include "helperfunctions.h"

using namespace std;

// Input the range in which to check if the files corresponding to the primes are in the data files
int main(int argc, char *argv[]) {
    int lower = atoi(argv[1]); //where you want to start with your primes
    int upper = atoi(argv[2]); //how high do you want to go with your primes

    std::vector<long> primes = generate_primes_in_range(lower, upper);

    vector<long> x; /*primes*/
    vector<float> y; /*Normalized Second Moment*/

    int a = 0;
    int b = 0;

    #pragma omp parallel for
    for (long p : primes) {
        string filename = "classdata/file_" + to_string(p) + ".csv";

        int MAX_ROWS;
        if (p % 4 == 1) MAX_ROWS = 4*p;
        else MAX_ROWS = 2*p;
        ifstream file(filename);
        
        if (!file.is_open()){
            cerr << "Error opening file for prime " <<  p << endl;
            continue;
        }
        
    }
    return 0;
}
