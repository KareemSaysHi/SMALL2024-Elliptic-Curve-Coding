#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h>
#include <cstdio>
#include <unordered_map>
#include <cassert>
#include "helperfunctions.h"

using namespace std;

vector<vector<long>> getPowersOfTmodP(long p, int highestPower) {
    vector<vector<long>> powers(p, vector<long>(highestPower, 0));
    for (long t = 0; t < p; t++) {
        for (int power = 0; power < highestPower; power++) {
            powers[t][power] = power_mod_p(t, power, p);
        }
    }
    return powers;
}

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
    int filterOn = atoi(argv[2]); //turns filter on or not
    int filterA = atoi(argv[3]); //A
    int filterB = atoi(argv[4]); //modB
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

    /*cout << power_mod_p(2, 45716, 91433) << "\n";
    return 69;*/
    
    std::vector<long> primes = generate_primes_in_range(2, n);

    vector<long> x; /*primes*/
    vector<float> y; /*Normalized Second Moment*/
    // vector<float> z; /*Weighted Running Average*/
    // vector<float> w; /*Normalized Second moment*/

    // float runningsum = 0;
    // float weightedrunningsum = 0;
    // int primecount = 0;
    // float weightedprimecount = 0;
    // int positivecount = 0;
    // int weightedpositivecount = 0;

    int a = 0;
    int b = 0;
    int highestPower = max(highestPowerA, highestPowerB);
    cout << "highestPower: " << highestPower << '\n';

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

    cout << "Polynomial you are running is " << "A = " + textA + ", B = " textB + '\n'; 

    #pragma omp parallel for
    for (int j = 0; j < primes.size(); j++) {
        long p = primes[j];
        
        vector<vector<long>> allPowersOfT = getPowersOfTmodP(p, highestPower + 1);

        if (filterOn) {
            if (p % filterB != filterA || p <= 5) {
                continue;
            }
        }

        long A = 0;
        long B = 0;
        int c = 0;
        long long value = 0;

        if (p % 100 == 1) cout << "on prime " << p << std::endl;

        

        string filename = "classdata/file_" + to_string(p) + ".csv";

        unordered_map<int,int> data;
        int dataFor0Bs[6];
        int dataFor0Aps[6];
        int x1;
        int y1;
        int lineCountGood = 0;

        if (FILE *fp = fopen(filename.c_str(), "r")) {
            while (fscanf(fp, "%d,%d", &x1, &y1) == 2) {
                dataFor0Bs[lineCountGood] = x1;
                dataFor0Aps[lineCountGood] = y1;
                lineCountGood ++;
            }
            data[0] = x1;
            lineCountGood = 1;
            while (fscanf(fp, "%d", &x1) == 1) {
                data[lineCountGood] = x1;
                lineCountGood ++ ;
            }
            fclose(fp);
        }

        int MAX_ROWS = p % 4 == 1 ? 4 * p : 2 * p; // expected number of rows
        if (abs(lineCountGood - MAX_ROWS) > 2) {
            cout << "File for prime " << p << " has " << lineCountGood << " lines. Expected approximately " << MAX_ROWS << ".\n";
            continue;
        }

        /*for (int i=0; i < MAX_ROWS; i++) {
            cout << data[i] << "\n";
        }
        continue;*/

        vector<int> reps = findQuarticResidueClasses(p);

        for (long t = 0; t < p; ++t) {
            // Calculate A and B
            A = calculatePoly(t, p, allPowersOfT, seqA);

            B = calculatePoly(t, p, allPowersOfT, seqB);
        
            //step 0: we need these guys to be greater than 0
            A = ((A % p) + p) % p;
            B = ((B % p) + p) % p;

            if (A == 0) {
                if (p % 3 == 2 || B == 0 || p == 3) continue;
                for (int i = 0; i < 6; i ++ ){
                    if (power_mod_p(multmodp(B, inverse(dataFor0Bs[i], p), p), (p-1)/6, p) == 1) { 
                        value += pow(dataFor0Aps[i], 2);
                        break;
                    }
                    if (i == 5) {
                        cout << "uh oh this is bad, p,A,B = " << p << "," << A  << "," << B << "\n";
                        for (int i = 0; i < 6; i++) {
                            cout << "index: " << i << " " << dataFor0Bs[i] << '\n';
                        }
                    }
                }
            }

            //step 1: figure out what residue class A is in
            for (int i = 1; i <= reps.size(); i++) { // i=0 will be reps[i] = 0, which is A = 0, dealt with above
                long lfourthmaybe = multmodp(A, inverse(reps[i], p), p);

                if (isFourthPower(lfourthmaybe, p)) {// at this point lfourthmaybe is a fourth power
                    //step 2: calculate lsixth
                    long lsquare = squareroot(lfourthmaybe, p);
                    long lsixth = multmodp(lsquare, lfourthmaybe, p);

                    //step 3: calculate B and lookup
                    B = multmodp(B, inverse(lsixth, p), p);

                    value += pow(data[p * (i-1) + B], 2);
                    break;
                }
            }
            //cout << "Prime: " << p << " t: " << t << " A: " << A  << " B: " << B << "value: " << value << '\n';
        }

        
        x.push_back(p);
        
        //subtract by p^2 and divide by p^(1.5) to normalize
        y.push_back((value - pow(p, 2)) / pow(p, 1.5));
    }
            
    // std::cout << "Finished with family " << outputfilename << endl;
    // std::cout << "Percent of time positive " << (positivecount * 1.0)/primecount << endl;

    std::ofstream output_file(outputfilename);
    if (!output_file.is_open()) {
        std::cout << "Unable to open output file" << std::endl;
        //return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        // output_file << x[i] << "," << y[i] << "," << z[i] << "," << w[i] << std::endl;
        output_file << x[i] << "," << y[i] << std::endl;
    }

    return 0;
}
