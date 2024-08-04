#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h>
#include <cassert>
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

vector<long> getBigGuysForRankSix(int p) {
    vector<long> bigA = {0, 5600, 4482, 6100, 891};
    vector<long> littleC = {0, 848, 1499, 2};
    vector<long> littleB = {9600, 7480, 6031, 1};
    vector<long> littleA = {1104, 6011, 166};
    vector<long> bigB = {2208, 1622, 8246, 5140, 1136, 8};
    vector<long> bigC = {384, 9352, 3214, 347, 9749, 264};
    vector<long> bigD = {3200, 1336, 4488, 4345, 759, 3431};

    long bigAmodp = largenummodp(bigA, p);
    long bigBmodp = largenummodp(bigB, p);
    long bigCmodp = largenummodp(bigC, p);
    long bigDmodp = largenummodp(bigD, p);
    long littleAmodp = largenummodp(littleA, p);
    long littleBmodp = largenummodp(littleB, p);
    long littleCmodp = largenummodp(littleC, p);

    vector<long> result = {bigAmodp, bigBmodp, bigCmodp, bigDmodp, littleAmodp, littleBmodp, littleCmodp};
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


    std::vector<long> primes = generate_primes_in_range(0, n);

    vector<int> x; /*primes*/
    vector<double> y; /*A-values*/

    #pragma omp parallel for
    for (int p : primes) {

        vector<vector<int>> allPowersOfT = getPowersOfTmodP(p, highestPower);

        // FOR RANK 6 STUFF
        vector<long> bigGuys = getBigGuysForRankSix(p);
        long bigA = bigGuys[0];
        long bigB = -1 * bigGuys[1];
        long bigC = bigGuys[2];
        long bigD = -1 * bigGuys[3];
        long littleA = bigGuys[4];
        long littleB = -1 * bigGuys[5];
        long littleC = bigGuys[6];


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
            /*int t_cubed = (((t * t) % p) * t) % p;
            int t_squared = (t * t) % p;
            A = 1;
            B = t_squared % p; */

            //x^3 + x + t^3
            //A = 1;
            //B = calculatePoly(t, p, allPowersOfT, {0, 0, 0, 1});

            //x^3 + x + (18th cyclotomic)
            //A = 2;
            //B = calculatePoly(t, p, allPowersOfT, {1, 0, 0, -1, 0, 0, 1});

            //RANK 6 STUFF
            A = (calculatePoly(t, p, allPowersOfT, {-1 * littleC, 2 * littleB}) * \
                (calculatePoly(t, p, allPowersOfT, {1 - bigA, 2, 1})) % p - \
                inverse(3, p) * calculatePoly(t, p, allPowersOfT, {-1 * bigB, 2 * littleA}) * \
                calculatePoly(t, p, allPowersOfT, {-1 * bigB, 2 * littleA}) % p) % p;
            A = (A+p) % p;

            B = 2 * (power_mod_p(inverse(3, p), 3, p) * power_mod_p(calculatePoly(t, p, allPowersOfT, {-1 * bigB, 2 * littleA}), 3, p) % p - \
                inverse(3, p) * calculatePoly(t, p, allPowersOfT, {-1 * bigB, 2 * littleA}) * calculatePoly(t, p, allPowersOfT, {-1 * bigC, 2 * littleB}) * \
                (calculatePoly(t, p, allPowersOfT, {1 - bigA, 2, 1})) % p + \
                calculatePoly(t, p, allPowersOfT, {-1 * bigD, 2 * littleC}) * power_mod_p(calculatePoly(t, p, allPowersOfT, {1 - bigA, 2, 1}), 2, p) % p) % p;
            B = (B+p) % p;

            /*A = 1;
            int num = 500;
            int res = 0; 

            int temp1 = (1 + power_mod_p(t, num, p));
            int temp2 = inverse(1+t, p);

            B = temp1 * temp2 % p;*/

            //cout << "A = " << A << ", B = " << B << endl;


            //step 0: we need these guys to be greater than 0
            A = ((A % p) + p) % p;
            B = ((B % p) + p) % p;

            if (A < 0 || B < 0) {
                while (true) {
                    cout << "A = " << A << ", B = " << B << endl;
                }
            }
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

            if (A < 0 || B < 0) {
                while (true) {
                    cout << "A = " << A << ", B = " << B << endl;
                }
            }

            readThing = get_value_from_file(p, repline, B, file);
            value += pow(readThing, 2);
        }
        cout << " p = " << p << ", sum = " << value << "\n";

        x.push_back(p);
        y.push_back((value - pow(p, 2)) / pow(p, 1.5)); //subtract by p^2 and divide by p^(3/2) to normalize
    
        file.close();
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