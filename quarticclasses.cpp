#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <omp.h> // used for parallel processing on Windows. If you are on a mac comment this out.
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
        cout << "finished legendre symbols for " + to_string(primes[p])+ "\n";  // using + to print out each line one at a time
    }

    // Write results to file
    #pragma omp parallel for
    for (long p = 0; p < numPrimes; p++) {
        //cout << "Starting prime " << prime << "\n";
        std::string filename = "classdata/file_" + std::to_string(primes[p]) + ".csv";
        
        ofstream file;
        file.open(filename);

        if (file.is_open()) {
            vector<int> residueClasses = findQuarticResidueClasses(primes[p]);
            int numResidueClasses = residueClasses.size();
            // do c=0 !! we don't store A = 0 B = 0 since a_p is always 0
            if (primes[p] % 3 == 1) {
                vector<long> vals;
                int numWritten = 0;
                for (long b = 1; b < primes[p]; b++) { //run over all bs
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
                            break; // we have already written it so who cares
                        }
                    }

                    // We haven't broken so we 
                    vals.push_back(result); // 1.add to seen vals 
                    file << b << "," << result << "\n"; // 2. and write to the file
                    numWritten++; // 3. increment counter
                }
                // numWritten stores the number of things put into vals which will always be 6 at the end
            }

            for (int c = 1; c < numResidueClasses; c++) { 
                // run over all residue classes, 
                // but dont do c=0!!!
                for (long b = 0; b < primes[p]; b++) { // run over all bs
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
            // tells you how many primes remaining
            // one could also change this to print out the prime it just finished

            // on Intel core i5 13500 takes about 30 minutes for 133000 to 133500

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
