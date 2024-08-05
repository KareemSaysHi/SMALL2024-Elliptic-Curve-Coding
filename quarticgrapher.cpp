#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
//#include <omp.h>
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
        result += long(coeff * powersOfT[t][count]);
        result = long(result % p);
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
    std::cout << highestPowerA << '\n';
    std::cout << highestPowerB << '\n';

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

    for (int i = 0; i <= highestPowerA; i++) {
        cout << "A index: " << i << " value: " << seqA[i] << '\n';
    }

    for (int i = 0; i <= highestPowerB; i++) {
        cout << "B index: " << i << " value: " << seqB[i] << '\n';
    }
    
    std::vector<long> primes = generate_primes_in_range(2, n);

    vector<long> x; /*primes*/
    vector<float> y; /*A-values*/
    vector<float> z; /*Weighted Running Average*/
    vector<float> w; /*Normalized Second moment*/

    float runningsum = 0;
    float weightedrunningsum = 0;
    int primecount = 0;
    float weightedprimecount = 0;
    int positivecount = 0;
    int weightedpositivecount = 0;

    int a = 0;
    int b = 0;
    int highestPower = max(highestPowerA, highestPowerB);
    cout << "highestPower: " << highestPower << '\n';

    //#pragma omp parallel for
    for (long p : primes) {
        vector<vector<long>> allPowersOfT = getPowersOfTmodP(p, highestPower + 1);

        if (filterOn) {
            if (p % filterB != filterA || p <= 5) {
                continue;
            }
        }

        long A = 0;
        long B = 0;
        int c = 0;
        long value = 0;
        int readThing = 0;
        int repline = 0;

        cout << "on prime " << p << std::endl;

        string filename = "classdata/file_" + to_string(p) + ".csv";
        cout << to_string(p);

        long MAX_ROWS = 5*p;
        ifstream file;
        file.open(filename);
        if(!file.is_open()){
            continue;
        }
    
        int data[MAX_ROWS];
        string line;
        int row = 0;
        while(getline(file,line) && row < MAX_ROWS+1){
            if (row == 0){
                row++;
            } else {
            stringstream ss(line);
            string cell;
            for (int i = 0; getline(ss, cell, ','); ++i) {
                if (i == 2) {
                    data[row-1] = stoi(cell);
                }
            }
            row++;
            }
        }
        file.close();
        vector<int> reps = findQuarticResidueClasses(p);

        for (long t = 0; t < p; ++t) {
            // Calculate A and B
            A = calculatePoly(t, p, allPowersOfT, seqA);

            B = calculatePoly(t, p, allPowersOfT, seqB);
        
            //step 0: we need these guys to be greater than 0
            A = ((A % p) + p) % p;
            B = ((B % p) + p) % p;

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
                    c = reps[i];
                    repline = i;
                    break;
                }
            }

            if (c == 0) {
                value += pow(data[B], 2);
            } else {

                long lfourth = A * (inverse(c, p) + p) % p;

                //step 2: calulcate lsixth
                long lsquare = squareroot(lfourth, p);
                //int lsquare = 1;
                long lsixth = (lsquare * lfourth) % p;

                //step 3: calculate B and lookup
                B = (B * inverse(lsixth, p)) % p;

                readThing = data[p * repline + B];
                value += pow(readThing, 2);
            }
        }
        x.push_back(p);
        float normalizedSecondMoment = (value - pow(p, 2)) / pow(p, 1.5); //subtract by p^2 and divide by p^(1.5) to normalize
        runningsum += normalizedSecondMoment;
        weightedrunningsum += normalizedSecondMoment * log(p);
        primecount += 1;
        weightedprimecount += log(p);

        // Calculate running average and weighted running average
        if (runningsum >= 0) {
            positivecount ++;
        }
        if (weightedrunningsum >= 0) {
            weightedpositivecount ++;
        }
        w.push_back(normalizedSecondMoment);
        y.push_back(runningsum/primecount); 
        z.push_back(weightedrunningsum/weightedprimecount);
    }
            
    std::string textA = "dataA";
    std::string textB = "B";
    for (int i = 0; i <= highestPowerA; i++) {
        // Polynomials are created so that seqA[i] is the coeff of the i^th degree term
        textA = textA + std::to_string(seqA[i]);
    }
    for (int i = 0; i <= highestPowerB; i++) {
        // Polynomials are created so that seqA[i] is the coeff of the i^th degree term
        textB = textB + std::to_string(seqB[i]);
    }

    std::string outputfilename = textA + textB + ".txt";
    std::cout << "Finished with family " << outputfilename << endl;
    std::cout << "Percent of time positive " << (positivecount * 1.0)/primecount << endl;

    std::ofstream output_file(outputfilename);
    if (!output_file.is_open()) {
        std::cout << "Unable to open output file" << std::endl;
        //return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        output_file << x[i] << "," << y[i] << "," << z[i] << "," << w[i] << std::endl;
    }

    return 0;
}