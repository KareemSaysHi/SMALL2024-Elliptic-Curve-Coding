#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <omp.h>
#include "helperfunctions.h"

using namespace std;

void compute_a_constants(long lower, long upper) {
    vector<long> primes = generate_primes_in_range(lower, upper);
    long numPrimes = primes.size();

    // Create a 2D array to store Legendre symbols
    //first index is the prime, second index whatever the input is
    
    vector<vector<int>> bigArray(upper-lower, vector<int>(upper, 0));
    //this is ok to be an int because Legendre symbol

    // Function to calculate Legendre symbols for each prime
    #pragma omp parallel for
    for (long p = 0; p < numPrimes; p++) {
        for (long x = 0; x < primes[p]; ++x) {
            bigArray[primes[p]-lower][x] = legendre_symbol(x, primes[p]);
        }
        cout << "finished legendre symbols for " << primes[p] << "\n"; 
    }
    // Write results to file
    #pragma omp parallel for
    for (long p = 0; p < numPrimes; p++) {
        //cout << "Starting prime " << prime << "\n";
        std::string filename = "classdata/file_" + std::to_string(primes[p]) + ".csv";
        
        ofstream file;
        file.open(filename);

        if (file.is_open()) {
            //file << findQuarticResidueClasses(prime) << "\n";
            //file << "a,b,value\n";
            // OK to be an int
            vector<int> residueClasses = findQuarticResidueClasses(primes[p]);
            int numResidueClasses = residueClasses.size();
            // do c=0 !!
                // don't store A = 0 B = 0 since always 0
            if (primes[p] % 3 == 1) {
                vector<long> vals;
                int numWritten = 0;
                bool flag = true;
                for (long b = 1; b < primes[p]; b++) { //run over all bs
                    flag = true;
                    // Check how many distinct values have been computed, if 6 we are finished
                    if (numWritten == 6){
                        break;
                    }

                    long result = 0;
                    // Compute value
                    for (long x = 0; x < primes[p]; x++) {
                        long value = (power_mod_p(x, 3, primes[p]) + b) % primes[p]; // A = 0 so no linear term
                        result += bigArray[primes[p]-lower][value];
                    }

                    // Check if computed value has been seen before
                    for (int i = 0; i < numWritten; i++) {
                        if (result == vals[i]){
                            flag = false;
                            break;
                        }
                    }

                    // If flag = true has not been seen before so we add to seen vals and write to the file and increment counter
                    if (flag) {
                        vals.push_back(result);
                        file << b << "," << result << "\n";
                        // numWritten stores the number of things put into vals which will always be 6
                        numWritten++;
                    }
                }
            }

            for (int c = 1; c < numResidueClasses; c++) { //run over all residue classes, 
            // dont do c=0!!!
                for (long b = 0; b < primes[p]; b++) { //run over all bs
                    long result = 0;
                    for (long x = 0; x < primes[p]; x++) {
                        long value = (power_mod_p(x, 3, primes[p]) + residueClasses[c] * x + b) % primes[p];
                        
                        result += bigArray[primes[p]-lower][value];
                        //result += power_mod_p(value ,int((prime - 1)/2), prime);
                    }
                    //file << residueClasses[c] << "," << b << "," << result << "\n";
                    file << result << '\n';
                }
            }
            cout << "Primes Remaining: " << --numPrimes << "\n";
            file.close();
        } else {
            cerr << "Unable to open file";
        }
    }
}

// Main function
int main(int argc, char *argv[]) {

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <file_name> <lower_limit> <upper_limit>" << endl;
        return 1;
    }

    long lower = stol(argv[1]);
    long upper = stol(argv[2]);

    compute_a_constants(lower, upper);
    
    return 0;

    // generate primes up to n
    
}
