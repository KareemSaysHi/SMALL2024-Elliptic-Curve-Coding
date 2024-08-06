#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h>
#include <cassert>
#include "helperfunctions.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]); //how high do you want to go with your primes
    int filterOn = atoi(argv[2]); //turns filter on or not
    int filterA = atoi(argv[3]); //A
    int filterB = atoi(argv[4]); //modB

    std::vector<long> primes = generate_primes_in_range(2, n);

    vector<long> x; /*primes*/
    vector<float> y; /*Normalized Second Moment*/

    int a = 0;
    int b = 0;

    #pragma omp parallel for
    for (long p : primes) {
        //vector<vector<long>> allPowersOfT = getPowersOfTmodP(p, highestPower + 1);

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

        int MAX_ROWS;
        if (p % 4 == 1) MAX_ROWS = 4*p;
        else MAX_ROWS = 2*p;
        ifstream file(filename);
        
        if (!file.is_open()){
            cerr << "Error opening file for prime " <<  p << endl;
            continue;
        }

        // int data[MAX_ROWS];
        // int dataFor0Bs[6];
        // int dataFor0Aps[6];
        // std::string line;

        // int lineCount = 0;
        // while (getline(file, line)) {
        //     std::stringstream ss(line);
        //     string a, b;

        //     if (lineCount < 6 && p % 3 == 1) {
        //         if (getline(ss, a, ',') && getline(ss, b)) {
        //             dataFor0Bs[lineCount] = stoi(a);
        //             dataFor0Aps[lineCount] = stoi(b);
        //         }
        //     } else //(lineCount < 6 + MAX_ROWS) 
        //     {
        //         if (getline(ss, a)) {
        //             if (p % 3 == 1) {
        //                 data[lineCount-6] = stoi(a);
        //             } else {
        //                 data[lineCount] = stoi(a);
        //             }
        //         }
        //     } 

        //     ++lineCount;
        // }
        // file.close();

        // // Check if file had correct number of lines
        // if (abs(lineCount - MAX_ROWS) > 7) {
        //     cout << "File for prime " << p << " has " << lineCount << " lines. Expected approximately " << MAX_ROWS << ".\n";
        //     continue;
        // }
    }

    return 0;
}