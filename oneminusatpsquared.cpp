#include "helperfunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

vector<int> calculateValues(int Acoeff1, int Acoeff2, int Bcoeff1, int Bcoeff2, int Apower1, int Apower2, int Bpower1, int Bpower2) {
    int value = 0;
    int A = 0;
    int B = 0;
    int returned = 0;

    vector<int> primes = generate_primes_in_range(0, 100);

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

void printFamily(int Acoeff1, int Acoeff2, int Bcoeff1, int Bcoeff2, int Apower1, int Apower2, int Bpower1, int Bpower2) {
    
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
        file << Acoeff1 << "T^" << Apower1 << " +  ";
    }
    if (Bcoeff2 != 0){
        file << Acoeff2 << "T^" << Apower2;
    }
    if (B)
    file << "\n";
}


int main() {
    vector<int> Acoeffs1 = {0, 1};
    vector<int> Acoeffs2 = {0, 1};
    vector<int> Bcoeffs1 = {0, 1};
    vector<int> Bcoeffs2 = {0, 1};
    int highestPowerA = 3;
    int highestPowerB = 5;

    ofstream file;
    file.open("superneatfile.txt");

    vector<int> primes = generate_primes_in_range(0, 100);

    if (file.is_open()) {
        for (int Apower1 = 0; Apower1 < highestPowerA+1; Apower1++) {
            cout << "Apower1 = " << Apower1 << "\n";
            for (int Apower2 = 0; Apower2 < Apower1; Apower2++) {
                for (int Bpower1 = 0; Bpower1 < highestPowerB+1; Bpower1++) {
                    for (int Bpower2 = 0; Bpower2 < Bpower1; Bpower2++) {
                        for (int Bcoeff1 = 0; Bcoeff1 < Bcoeffs1.size(); Bcoeff1++) {
                            for (int Bcoeff2 = 0; Bcoeff2 < Bcoeffs2.size(); Bcoeff2++) {
                                for (int Acoeff1 = 0; Acoeff1 < Acoeffs1.size(); Acoeff1++) {
                                    for (int Acoeff2 = 0; Acoeff2 < Acoeffs2.size(); Acoeff2++) {
                                        if (Acoeff1 == 0 && Apower1 != 0) {
                                            continue;
                                        }
                                        if (Acoeff2 == 0 && Apower2 != 0) {
                                            continue;
                                        }
                                        if (Bcoeff1 == 0 && Bpower1 != 0) {
                                            continue;
                                        }
                                        if (Bcoeff2 == 0 && Bpower2 != 0) {
                                            continue;
                                        }
                                        
                                        bool printFamily = true;
                                        
                                        vector<int> values = calculateValues(Acoeff1, Acoeff2, Bcoeff1, Bcoeff2, Apower1, Apower2, Bpower1, Bpower2);
                                        
                                        //cout << "A = " << Acoeff1 << "T^" << Apower1 << " +  " << Acoeff2 << "T^" << Apower2 << "\n";
                                        //cout << "B = " << Bcoeff1 << "T^" << Bpower1 << " +  " << Bcoeff2 << "T^" << Bpower2 << "\n";
                                        //file << "A = " << Acoeff1 << "T^" << Apower1 << " +  " << Acoeff2 << "T^" << Apower2 << "\n";
                                        //file << "B = " << Bcoeff1 << "T^" << Bpower1 << " +  " << Bcoeff2 << "T^" << Bpower2 << "\n";
                                        /*for (int i = 0; i < values.size(); i++) {
                                            file << primes[i] << "," << values[i] << "\n";
                                        }
                                        file << "\n";*/

                                        for (int i = 0; i < values.size(); i++) {
                                            if (values[i] != 1) {
                                                printFamily = false;
                                                break;
                                            }
                                        }

                                        if (printFamily) {
                                            printFamily(Acoeff1, Acoeff2, Bcoeff1, Bcoeff2, Apower1, Apower2, Bpower1, Bpower2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    file.close();

    } else {
        cerr << "Unable to open file";
    }

    return 0;
}


