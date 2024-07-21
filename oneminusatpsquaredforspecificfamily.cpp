#include "helperfunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

vector<int> calculateValues(int Acoeff1, int Acoeff2, int Bcoeff1, int Bcoeff2, int Apower1, int Apower2, int Bpower1, int Bpower2, ofstream &file) {
    int value = 0;
    int A = 0;
    int B = 0;
    int returned = 0;

    vector<int> primes = generate_primes_in_range(0, 1000);

    vector<int> setofvalues = {};
    
    for (int p : primes) {
        value = 0;
        for (int t = 0; t < p; t++) {
            A = (Acoeff1 * power_mod_p(t, Apower1, p) + Acoeff2 * power_mod_p(t, Apower2, p)) % p;
            B = (Bcoeff1 * power_mod_p(t, Bpower1, p) + Bcoeff2 * power_mod_p(t, Bpower2, p)) % p;
            //cout << "t = " << t << "\n";
            //cout << "p = " << p << " A = " << A << " B = " << B << "\n";
            returned = getAp(p, A, B);
            //cout << "Returned = " << returned << "\n";
            value += power_mod_p(1 - returned, 2, p);
        }
        setofvalues.push_back((value +1)%p);  //pico tim marie-helene kareem found plus one
    }
    
    return setofvalues; 
    
}

/*void printFamily(int Acoeff1, int Acoeff2, int Bcoeff1, int Bcoeff2, int Apower1, int Apower2, int Bpower1, int Bpower2, ofstream &file) {
    
    file << "A = ";
    if (Acoeff1 != 0) {
        file << Acoeff1 << "T^" << Apower1 << " +  ";
    }
    if (Acoeff2 != 0){
        file << Acoeff2 << "T^" << Apower2;
    }
    if (Acoeff1 == 0 && Acoeff2 == 0) {
        file << "0";
    }
    file << "B = ";
    if (Bcoeff1 != 0) {
        file << Bcoeff1 << "T^" << Bpower1 << " +  ";
    }
    if (Bcoeff2 != 0){
        file << Bcoeff2 << "T^" << Bpower2 << " +  ";
    }
    if (Bcoeff1 == 0 && Bcoeff2 == 0) {
        file << "0";
    }
    file << "\n";
}*/


int main() {
    vector<int> Acoeffs1 = {0, 1};
    vector<int> Acoeffs2 = {0, 1};
    vector<int> Bcoeffs1 = {0, 1};
    vector<int> Bcoeffs2 = {0, 1};
    int highestPowerA = 3;
    int highestPowerB = 5;

    ofstream file;
    file.open("x3+x+t2+1.txt");

    vector<int> primes = generate_primes_in_range(0, 1000);

    if (file.is_open()) {
        vector<int> values = calculateValues(1, 0, 1, 1, 0, 0, 2, 0, file);
        //printFamily(1, 0, 1, 0, 0, 0, 2, 0, file);
        for (int i = 0; i < values.size(); i++) {
            file << primes[i] << "," << values[i] << "\n";
        }
    }
    file.close();

    return 0;
}


