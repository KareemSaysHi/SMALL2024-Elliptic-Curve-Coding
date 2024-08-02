#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include "helperfunctions.h"
#include <omp.h>

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

    double thresh = 0.95; // threshold for what to write to output file if positive

    int n = atoi(argv[1]); //how high do you want to go with your primes
    int filterOn = atoi(argv[2]); //turns filter on or not
    int filterA = atoi(argv[3]); //A
    int filterB = atoi(argv[4]); //modB

    std::vector<int> primes = generate_primes_in_range(2, n);

    std::vector<int> nums = {0, 1};
    int length = 6; // Length of sequences to generate so desired degree - 1
    
    // Generate sequences of coefficients
    // Code reads coeffs for A as going from degree 0 to degree (length - 1) from left to right
    std::vector<std::vector<int>> coeffs = generateAllSequences(nums, length);

    // Code for testing particular sequence to do so change
    // Outer for loop to go over coeffs1 and inner for loop to go over coeffs2 
    /*std::vector<int> Alist;
    Alist.push_back(1);
    Alist.push_back(0);
    Alist.push_back(0);
    Alist.push_back(0);
    std::vector<std::vector<int>> coeffs1;
    coeffs1.push_back(Alist);
    
    std::vector<int> Blist;
    Blist.push_back(0);
    Blist.push_back(0);
    Blist.push_back(0);
    Blist.push_back(1);
    std::vector<std::vector<int>> coeffs2;
    coeffs2.push_back(Blist);
    */
    #pragma omp parallel for

    for (const auto& seqA : coeffs) {
        // For loop to choose A polynomial
        // Exclude all zero sequence corresponding to A = 0
        bool flag = true;
        for (int i = 0; i < length; i++){
            if (seqA[i] != 0) {
                flag = false;
                cout << "Entered" << endl;
                break;
            }
        }
        if (flag){
            continue;
        }

        for (const auto& seqB : coeffs) {
            //cout << "hello" << endl;
             // Exclude all zero sequence corresponding to B = 0
            bool flag = true;
            for (int i = 0; i < length; i++){
                if (seqB[i] != 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                continue;
            }

            // Reinitialize for each family
            vector<int> x; /*Primes*/
            vector<double> y; /*Running Average*/
            vector<double> z; /*Weighted Running Average*/
            vector<double> w; /*Normalized Second moment*/

            float runningsum = 0;
            float weightedrunningsum = 0;
            int primecount = 0;
            float weightedprimecount = 0;
            int positivecount = 0;
            int weightedpositivecount = 0;

            // For loop to choose B polynomial

            
            for (int p : primes) {
                if (filterOn) {
                    if (p % filterB != filterA) {
                        continue;
                    }
                }

                int c = 0;
                int value = 0;
                int readThing = 0;
                int repline = 0;

                string filename = "classdata/file_" + to_string(p) + ".csv";

                int MAX_ROWS = 5*p;
                ifstream file(filename);
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
                    // Generate polynomials
                    long A = 0;
                    long B = 0;
                    long powT = 0;
                    for (int i = 0; i < length; i++) {
                        // Polynomials created so that seqA[i] is the coeff of the i^th degree term
                        if (i == 0) {
                            powT = 1;
                        } else {
                            powT = power_mod_p(t, i, p);
                        }
                        A = (A + (seqA[i] * powT) % p) % p;
                        B = (B + (seqB[i] * powT) % p) % p;
                    }
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

                        int lfourth = A * (inverse(c, p) + p) % p;

                        //step 2: calulcate lsixth
                        int lsquare = squareroot(lfourth, p);
                        //int lsquare = 1;
                        int lsixth = (lsquare * lfourth) % p;

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
            
            // Only write to output file if positive >= thresh
            if ((positivecount * 1.0)/primecount >= thresh) {
                std::string textA = "dataA";
                std::string textB = "B";
                for (int i = 0; i < length; i++) {
                    // Polynomials are created so that seqA[i] is the coeff of the i^th degree term
                    textA = textA + std::to_string(seqA[i]);
                    textB = textB + std::to_string(seqB[i]);
                }
                std::string outputfilename = "familysearchdata/" + textA + textB + ".txt";
                cout << "Finished with family " << outputfilename << endl;
                cout << "Percent of time positive " << (positivecount * 1.0)/primecount << endl;

                std::ofstream output_file(outputfilename);
                if (!output_file.is_open()) {
                    cout << "Unable to open output file" << std::endl;
                    //return 1;
                }

                for (size_t i = 0; i < x.size(); ++i) {
                    output_file << x[i] << "," << y[i] << "," << z[i] << "," << w[i] << std::endl;
                }
            }
        }
    }
    return 0;
}
