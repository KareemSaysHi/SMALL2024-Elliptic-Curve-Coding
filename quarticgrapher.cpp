#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <omp.h>
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

long calculatePoly(int t, int p, const vector<vector<long>>& powersOfT, const vector<int>& coeffs) {
    long result = 0;
    int count = 0;
    for (int coeff: coeffs) {
        result += coeff * powersOfT[t][count];
        result = result % p;
        count++;
    }
    return result;
}

vector<long> getBigGuysForRankSix(long p) {
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


    //std::vector<long> primes = generate_primes_in_range(0, n);
    std::vector<long> primes;// = [2,3,5,7,11];
    /*primes.push_back(2);
    primes.push_back(3);
    primes.push_back(5);
    primes.push_back(7);*/
    primes.push_back(11);
    //primes.push_back(13);
    //primes.push_back(17);

    vector<int> x; /*primes*/
    vector<double> y; /*A-values*/
    vector<double> z; /*A-values*/
    vector<float> secondMoment; /*A-values*/

    float runningsum = 0;
    float weightedrunningsum = 0;
    int primecount = 0;
    float weightedprimecount = 0;
    int positivecount = 0;
    int weightedpositivecount = 0;


    auto start = std::chrono::system_clock::now();

    #pragma omp parallel for
    for (long p : primes) {

        vector<vector<long>> allPowersOfT = getPowersOfTmodP(p, highestPower);

        // FOR RANK 6 STUFF
        /*vector<long> bigGuys = getBigGuysForRankSix(p);
        long bigA = bigGuys[0];
        long bigB = -1 * bigGuys[1];
        long bigC = bigGuys[2];
        long bigD = -1 * bigGuys[3];
        long littleA = bigGuys[4];
        long littleB = -1 * bigGuys[5];
        long littleC = bigGuys[6];*/

        if (filterOn) {
            if (p % filterB != filterA) {
                continue;
            }
        }
        if (p <= 5) continue;

        int A = 0;
        int B = 0;
        int c = 0;
        int value = 0;
        int repline = 0;

        //cout << "on prime " << p << std::endl;                
        string filename = "classdata/file_" + to_string(p) + ".csv";

        int MAX_ROWS;
        if (p % 4 == 1) MAX_ROWS = 4*p;
        else MAX_ROWS = 2*p;
        ifstream file(filename);
        
        if (!file.is_open()){
            cerr << "Error opening file!" << endl;
            return 1;
        }

        int data[MAX_ROWS];
        int dataFor0Bs[6];
        int dataFor0Aps[6];
        std::string line;

        int lineCount = 0;
        while (getline(file, line)) {
            std::stringstream ss(line);
            string a, b;

            if (lineCount < 6 && p == 1 % 3) {
                if (getline(ss, a, ',') && getline(ss, b)) {
                    dataFor0Bs[lineCount] = stoi(a);
                    dataFor0Aps[lineCount] = stoi(b);
                }
            } else //(lineCount < 6 + MAX_ROWS) 
            {
                if (getline(ss, a)) {
                    data[lineCount-6] = stoi(a);
                }
            } 

            ++lineCount;
        }
        file.close();
        
        vector<int> reps = findQuarticResidueClasses(p);

        for (long t = 0; t < p; ++t) {
            //int t2 = power_mod_p(t, 2, p);
            long t3 = power_mod_p(t, 3, p);
            //int t4 = power_mod_p(t, 4, p);
            //int t5 = power_mod_p(t, 5, p);
            //int t6 = power_mod_p(t, 6, p);
            //int t7 = power_mod_p(t, 7, p);
            //int t8 = power_mod_p(t, 8, p);
            long t9 = power_mod_p(t, 9, p);
            //int t10= power_mod_p(t, 10, p);

            A = 1;
            B = 1;


            // Compute Pythagorean family
            /*int poly = 2 * t * inverse(power_mod_p(t,2,p)-1,p);
            int b = - power_mod_p(poly,2,p) + 1;
            int c = power_mod_p(poly,4,p);
            A = ((c - inverse(3,p) * power_mod_p(b, 2, p)) % p + p)%p;
            B = ((2 * inverse(27,p) * power_mod_p(b,3,p) - c * b * inverse(3,p))%p + p)%p;*/
            
            A = ((A % p) + p) % p;
            B = ((B % p) + p) % p;

            if (A == 0) {
                if (p == 2 % 3) continue;
                for (int i = 0; i <= 6; i ++ ){
                    if (power_mod_p((B * inverse(dataFor0Bs[i], p)) % p, (p-1)/6, p) == 1) {
                        value += pow(dataFor0Aps[i], 2);
                        break;
                    }
                    if (i == 6) {
                        cout << "uh oh this is bad, p,A,B = " << p << "," << A  << "," << B << "\n";
                    }
                }
            }

            //step 1: figure out what residue class A is in
            for (int i = 1; i <= reps.size(); i++) { // i=0 will be reps[i] = 0, which is A = 0, dealt with above
                long lfourthmaybe = (A * inverse(reps[i], p)) % p;

                if (isFourthPower(lfourthmaybe, p)) {// at this point lfourthmaybe is a fourth power
                    //step 2: calculate lsixth
                    long lsquare = squareroot(lfourthmaybe, p);
                    long lsixth = (lsquare * lfourthmaybe) % p;

                    //step 3: calculate B and lookup
                    B = (B * (inverse(lsixth, p)+p)) % p;

                    value += pow(data[p * (i-1) + B], 2);
                    break;
                }
            }
        }
        //cout << " p = " << p << ", sum = " << value << "\n";

        x.push_back(p);
        float normalizedSecondMoment = (value - pow(p, 2)) / pow(p, 1.5); //subtract by p^2 and divide by p^(1.5) to normalize

        runningsum += normalizedSecondMoment;
        weightedrunningsum += normalizedSecondMoment * log(p);
        primecount += 1;
        weightedprimecount += log(p);

        if (runningsum >= 0) positivecount ++;
        if (weightedrunningsum >= 0) weightedpositivecount ++;

        y.push_back(runningsum/primecount); 
        z.push_back(weightedrunningsum/weightedprimecount); 
        secondMoment.push_back(normalizedSecondMoment);

        if (primecount % 100 == 0) {
            auto now = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = now-start;
            cout << "Just finished p=" 
                << p 
                << " which is the " 
                << primecount 
                << "th prime, time elapsed thusfar = " 
                << elapsed_seconds.count() << "s " 
                << 1.0 * positivecount / primecount 
                << " and the current running average is "
                << 1.0 * runningsum / primecount
                << "\n"; // and took " << elapsed_seconds.count() << " for this gap\n";
            //last = end;
        }

        file.close();
    }
    cout << "Running average is positive " << 1.0 * positivecount / primecount  << " and weighted running average is positive " << 1.0 * weightedpositivecount/primecount << "\n";

    std::ofstream output_file("data.txt");
    if (!output_file.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        output_file << x[i] << "," << y[i] << "," << z[i] << "," << secondMoment[i] << std::endl;
    }

    return 0;
}
