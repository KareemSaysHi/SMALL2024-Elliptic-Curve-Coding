#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h>
#include <cassert>
#include <unordered_map>
#include "helperfunctions.h"

using namespace std;

// Input the range in which to check if the files corresponding to the primes are in the data files
int main(int argc, char *argv[]) {
    int lower = atoi(argv[1]); //where you want to start with your primes
    int upper = atoi(argv[2]); //how high do you want to go with your primes

    std::vector<long> primes = generate_primes_in_range(lower, upper);

    vector<long> x; /*primes*/

    int a = 0;
    int b = 0;

    #pragma omp parallel for
    for (long p : primes) {
        long A = 0;
        long B = 0;
        int c = 0;
        long value = 0;
        int readThing = 0;
        int repline = 0;

        //if (p % 100 == 1) cout << "on prime " << p << std::endl;

        string filename = "classdata/file_" + to_string(p) + ".csv";

        long MAX_ROWS;
        if (p % 4 == 1) MAX_ROWS = 4*p;
        else MAX_ROWS = 2*p;
        ifstream file(filename);
        
        if (!file.is_open()){
            cerr << "Error opening file for prime " <<  p << endl;
            continue;
        }

        unordered_map<int,int> data;
        int dataFor0Bs[6];
        int dataFor0Aps[6];
        std::string line;

        int lineCount = 0;
        while (getline(file, line)) {
            std::stringstream ss(line);
            string a, b;

            if (lineCount < 6 && p % 3 == 1) {
                if (getline(ss, a, ',') && getline(ss, b)) {
                    dataFor0Bs[lineCount] = stoi(a);
                    dataFor0Aps[lineCount] = stoi(b);
                }
            } else //(lineCount < 6 + MAX_ROWS) 
            {
                if (getline(ss, a)) {
                    if (p % 3 == 1) {
                        data[lineCount-6] = stoi(a);
                    } else {
                        data[lineCount] = stoi(a);
                    }
                }
            } 

            ++lineCount;
        }
        file.close();

        // Check if file had correct number of lines
        if (abs(lineCount - MAX_ROWS) > 7) {
            cout << "File for prime " << p << " has " << lineCount << " lines. Expected approximately " << MAX_ROWS << ".\n";
            continue;
        }
        
    }
    return 0;
}
