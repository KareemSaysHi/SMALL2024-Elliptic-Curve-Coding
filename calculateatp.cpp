#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <math.h>
//#include <omp.h>
#include "helperfunctions.h"

using namespace std;

vector<vector<int>> getPowersOfTmodP(int p, int highestPower) {
    vector<vector<int>> powers(p, vector<int>(highestPower, 0));
    for (int t = 0; t < p; t++) {
        for (int power = 0; power < highestPower; power++) {
            powers[t][power] = power_mod_p(t, power, p);
        }
    }
    return powers;
}

int calculatePoly(int t, int p, const vector<vector<int>>& powersOfT, const vector<int>& coeffs) {
    int result = 0;
    int count = 0;
    for (int coeff: coeffs) {
        result += coeff * powersOfT[t][count];
        result = result % p;
        count++;
    }
    return result;
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file_name> <upper_limit> <filter_on> <a> <mod b>" << endl;
        return 1;
    }

    int n = atoi(argv[1]); //how high do you want to go with your primes
    int filterOn = atoi(argv[2]); //turns filter on or not
    int filterA = atoi(argv[3]); //A
    int filterB = atoi(argv[4]); //modB

    int highestPower = 120;


    std::vector<int> primes = generate_primes_in_range(0, n);

    vector<int> x; /*primes*/
    vector<double> y; /*A-values*/

    //#pragma omp parallel for
    for (int p : primes) {

        vector<vector<int>> allPowersOfT = getPowersOfTmodP(p, highestPower);

        if (filterOn) {
            if (p % filterB != filterA || p <= 5) {
                continue;
            }
        }

        int A = 0;
        int B = 0;
        int c = 0;
        int value = 0;
        int readThing = 0;
        int repline = 0;

        cout << "on prime " << p << std::endl;

        string filename = "classdata/file_" + to_string(p) + ".csv";

        ifstream file;
        file.open(filename);

        
        vector<int> reps = findQuarticResidueClasses(p);

        for (int t = 0; t < p; ++t) {
            
            //here we'll be looking at x^3 + A(T)x + B(T)

            //define the function we're going to be looking at
            A = 1;
            B = calculatePoly(t, p, allPowersOfT, {0, 0, 1}); //t^2

            //step 1: figure out what residue class A is in
            for (int i = 0; i <= reps.size(); i++) {
                if (reps[i] == 0) {
                    if (A == 0) {
                        c = 0;
                        repline = i;
                        break;
                    }
                }

                if (isFourthPower((A * inverse(reps[i], p)) % p, p)) {
                    //cout << "found a fourth power - " << reps[i] << endl;
                    //cout << (A * inverse(reps[i], p)) % p << endl;
                    //cout << isFourthPower((A * inverse(reps[i], p)) % p, p) << endl;
                    c = reps[i];
                    repline = i;
                    break;
                }
            }

            if (c == 0) {
                readThing = get_value_from_file(p, repline, B, file);
                value += pow(readThing, 2); 
                continue;
            }

            int lfourth = A * inverse(c, p) % p;

            //step 2: calulcate lsixth
            int lsquare = squareroot(lfourth, p);
            //int lsquare = 1;
            int lsixth = lsquare * lfourth % p;

            //step 3: calculate B and lookup
            B = B * inverse(lsixth, p) % p;

            readThing = get_value_from_file(p, repline, B, file);
            value += pow(readThing, 2);
        }
        cout << " p = " << p << ", sum = " << value << "\n";

        x.push_back(p);
        y.push_back(value);
    
        file.close();
    }

    std::ofstream output_file("x^2+x+t^2 - a_tp^2.txt");
    if (!output_file.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        output_file << x[i] << "," << y[i] << std::endl;
    }

    return 0;
}