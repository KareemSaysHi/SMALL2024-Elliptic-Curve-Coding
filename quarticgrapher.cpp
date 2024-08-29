#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h> // used for parallel processing on Windows. If you are on a mac comment this out.
#include <cstdio>
#include <unordered_map>
#include <cassert>
#include "helperfunctions.h"

using namespace std;

// computes the powers of t mod $p$ until highestPower
vector<vector<long>> getPowersOfTmodP(long p, int highestPower) {
    vector<vector<long>> powers(p, vector<long>(highestPower, 0));
    for (long t = 0; t < p; t++) {
        for (int power = 0; power < highestPower; power++) {
            powers[t][power] = power_mod_p(t, power, p);
        }
    }
    return powers;
    // n.b. this could be implemented better but it is not nearly the slowest part so this suffices
}

// computes the polynomial in t mod $p$
long calculatePoly(long t, long p, const vector<vector<long>>& powersOfT, const vector<int>& coeffs) {
    long result = 0;
    int count = 0;
    for (int coeff: coeffs) {
        result += multmodp(coeff, powersOfT[t][count], p);
        result = result % p;
        count++;
    }
    return long(result);
}


int main(int argc, char *argv[]) {
    int n = atoi(argv[1]); //how high do you want to go with your primes
    int filterOn = atoi(argv[2]); //turns filter on or not (i.e. if 1 then on, if 1 then off)
    int filterA = atoi(argv[3]); //A
    int filterB = atoi(argv[4]); //modB
    // the above two lines, if filterOn is true force primes to be filterA mod filterB

    int highestPowerA = atoi(argv[5]); // highest power of A so (length - 1) of seqA
    int highestPowerB = atoi(argv[6]); // highest power of B so (length - 1) of seqB

    // read in coeff sequences for A and B from command line
    vector<int> seqA;
    vector<int> seqB;
    int start = 7;
    for (int i = start; i <= highestPowerA + start; i++) {
        seqA.push_back(atoi(argv[i]));
    }

    start = 7 + highestPowerA + 1;
    for (int i = start; i <= highestPowerB + start; i++) {
        seqB.push_back(atoi(argv[i]));
    }

    std::vector<long> primes = generate_primes_in_range(2, n); 
    // generate the primes up to n, can change lower bound if you care about less.

    vector<long> x; /*primes*/
    vector<float> y; /*Normalized Second Moment*/

    int a = 0;
    int b = 0;
    int highestPower = max(highestPowerA, highestPowerB);
    cout << "highestPower: " << highestPower << '\n'; // print out the highest power for confirmation

    std::string textA;
    std::string textB;
    for (int i = 0; i <= highestPowerA; i++) {
        // Polynomials are created so that seqA[i] is the coeff of the i^th degree term
        textA = textA + std::to_string(seqA[i]);
    }
    for (int i = 0; i <= highestPowerB; i++) {
        // Polynomials are created so that seqA[i] is the coeff of the i^th degree term
        textB = textB + std::to_string(seqB[i]);
    }

    std::string outputfilename = "dataA" + textA + "B" + textB + ".txt";

    cout << "Polynomial you are running is " << "A = " + textA + ", B = " + textB + '\n'; 
    // print out the polynomial confirmation
    // e.g. the polynomial 3x^2 + 7x + 9 would be 973
    // e.g. the polynomial 4x^5 + 1 would be 100004

    #pragma omp parallel for
    for (int j = 0; j < primes.size(); j++) { // we loop this way because the other way doesn't work in this version of c++
        long p = primes[j];
        
        vector<vector<long>> allPowersOfT = getPowersOfTmodP(p, highestPower + 1);

        if (filterOn) {
            if (p % filterB != filterA) {
                continue;
            }
        }

        long A = 0;
        long B = 0;
        int c = 0;
        long long value = 0;

        if (p % 100 == 1) cout << "on prime " << p << std::endl;
        // this is one nice way to track your progress
        // this prints out 1/40 of the primes being run
        // feel free to change to a different value or something similar

        string filename = "classdata/file_" + to_string(p) + ".csv";

        unordered_map<int,int> data; 
        // this is a hashmap
        // it is important because otherwise you may have memory issues when requesting p=120k+
        int dataFor0Bs[6];
        int dataFor0Aps[6];
        int x1; // these two lines are necessary
        int y1; // so the following code knows the correct types

        int lineCount = 0;

        if (FILE *fp = fopen(filename.c_str(), "r")) {
            while (fscanf(fp, "%d,%d", &x1, &y1) == 2) { // the first 6 rows are read in like such
                dataFor0Bs[lineCount] = x1;
                dataFor0Aps[lineCount] = y1;
                lineCount ++;
            }
            data[0] = x1; // the while loop will be false but still put data into x1, we need that data
            lineCount = 1;
            while (fscanf(fp, "%d", &x1) == 1) { // the rest of the rows are read in
                data[lineCount] = x1;
                lineCount ++;
            }
            fclose(fp); // closes the file.
        }

        int MAX_ROWS = p % 4 == 1 ? 4 * p : 2 * p; // the expected number of rows
        if (abs(lineCount - MAX_ROWS) > 7) {
            cout << "File for prime " << p << " has " << lineCount << " lines. Expected approximately " << MAX_ROWS << ".\n";
            continue;
        }

        // code for bugtesting to see if data is correct
        /*for (int i=0; i < MAX_ROWS; i++) {
            cout << data[i] << "\n";
        }
        continue;*/

        vector<int> reps = findQuarticResidueClasses(p);
        for (long t = 0; t < p; ++t) {
            // Calculate A and B
            A = calculatePoly(t, p, allPowersOfT, seqA);
            B = calculatePoly(t, p, allPowersOfT, seqB);
        
            //step 0: we need 0<= A,B < p
            A = ((A % p) + p) % p;
            B = ((B % p) + p) % p;

            if (A == 0) { // A=0 code
                if (p % 3 == 2 || B == 0 || p == 3) continue;
                // if p is not 1 mod 3 then a_p is 0
                // alternatively, if B is 0, then a_p is 0
                // hence either way we can continue

                for (int i = 0; i < 6; i ++ ){
                    if (power_mod_p(multmodp(B, inverse(dataFor0Bs[i], p), p), (p-1)/6, p) == 1) { 
                        // check if B/dataFor0Bs[i] is a 6th power
                        value += pow(dataFor0Aps[i], 2);
                        break;
                    }
                    if (i == 5) { // hopefully you never get to this point!!
                        cout << "uh oh this is bad, p,A,B = " << p << "," << A  << "," << B << "\n";
                        for (int i = 0; i < 6; i++) {
                            cout << "index: " << i << " " << dataFor0Bs[i] << '\n';
                        }
                    }
                }
            }

            //step 1: figure out what residue class A is in
            for (int i = 1; i <= reps.size(); i++) { 
                // i = 0 will be reps[i] = 0, which is A = 0, which is dealt with above
                long lfourthmaybe = multmodp(A, inverse(reps[i], p), p);

                if (isFourthPower(lfourthmaybe, p)) { 
                    // at this point lfourthmaybe is a fourth power
                    // step 2: calculate lsixth
                    long lsquare = squareroot(lfourthmaybe, p);
                    long lsixth = multmodp(lsquare, lfourthmaybe, p);

                    //step 3: calculate B and lookup
                    B = multmodp(B, inverse(lsixth, p), p);

                    value += pow(data[p * (i-1) + B], 2);
                    break;
                }
            }
            //cout << "Prime: " << p << " t: " << t << " A: " << A  << " B: " << B << "value: " << value << '\n';
            // the above line of code is for testing
        }
        x.push_back(p);
        y.push_back((value - pow(p, 2)) / pow(p, 1.5)); //subtract by p^2 and divide by p^(1.5) to normalize
    }
            
    // cout << "Finished computing second moments\n";

    std::ofstream output_file(outputfilename);
    if (!output_file.is_open()) { // ideally this never runs
        cout << "Unable to open output file" << std::endl;
        //return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        // output_file << x[i] << "," << y[i] << "," << z[i] << "," << w[i] << std::endl;
        output_file << x[i] << "," << y[i] << std::endl; 
        // writes p, [normalized second moment of p]
    }

    return 0;
}
