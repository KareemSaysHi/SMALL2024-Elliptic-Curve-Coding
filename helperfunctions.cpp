#include "helperfunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Function to calculate a^b mod p without overflow
long power_mod_p(long a, int b, int p) {
    long long arcst = (long long) a; //Cast a to long long data type to avoid overflow when squaring
    long long result = 1;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result * arcst) % p;
        }

        b = b >> 1;
        arcst = (arsct * arcst) % p;
    }
    return (long) (((result % p) + p ) % p);
}

// Function to calculate Legendre symbol
int legendre_symbol(int x, int p) {
    if (x % p == 0) {
        return 0;
    } else if (power_mod_p(x, int((p-1)/2), p) == 1) {
        return 1;
    } else {
        return -1;
    }
}

//find inverse of a mod p
int inverse(int a, int p) {
    return power_mod_p(a, p-2, p);
}

// Returs true if fourth power mod p, otherwise returns false
bool isFourthPower(int x, int p) {
    if (p % 4 == 1) {
        if (power_mod_p(x, int((p-1)/4), p) == 1) { //1 iff x is a fourth power mod p since multiplicative group is cyclic
            return true;
        }
    } else {
        if (power_mod_p(x, int((p-1)/2), p) == 1) { //in 3 mod 4, if you're a square, you're a fourth power!
            return true;
        }
    }
    return false;
}

vector<int> findQuarticResidueClasses(int p) {
    //need to make this work for p = 2, 3, maybe 5
    if (p == 2) {
        return vector<int>{0, 1};
    } else if (p == 3) {
        return vector<int>{0, 1, 2};
    } else if (p == 5) {
        return vector<int>{0, 1, 2, 3, 4}; //all in different
    }
    int thingToCheck = 0;
    bool inPrevResClass = false;
    vector<int> classReps;
    classReps.push_back(0);
    for (int i = 1; i < p; i++) {
        inPrevResClass = false;
        for (int rep : classReps) {
            thingToCheck = i * inverse(rep, p) % p;
            if (isFourthPower(thingToCheck, p)) {
                inPrevResClass = true;
                break;
            }
        }

        if (!inPrevResClass) {
            classReps.push_back(i);
        }
            
        if (p % 4 == 3) {
            if (classReps.size() >= 3) { //we have 2 quartic classes in 3 mod 4
                return classReps;
            }
        } else if (p % 4 == 1) {
            if (classReps.size() >= 5) { //we have 4 quartic class in 1 mod 4
                return classReps;
            }
        }
        
    }

    cout << "something very bad has happened if this is printed" << endl;
    return classReps;
}

// Function to generate prime numbers up to k
vector<int> generate_primes_in_range(int lower, int upper) {
    vector<int> primes;
    for (int num = 2; num < upper; num++) {
        bool is_prime = true;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime && (num >= lower)) {
            primes.push_back(num);
        }
    }
    return primes;
}

static vector<int> cipollamult(vector<int>& term1, vector<int>& term2, int fieldextension, int p) {
    // Ensure the input vectors are of size 2
    if (term1.size() != 2 || term2.size() != 2) {
        throw std::invalid_argument("Both input vectors must be of size 2.");
    }
    
    int a = term1[0];
    int b = term1[1];
    int c = term2[0];
    int d = term2[1];

    vector<int> result = {( a * c % p + (((fieldextension * b) % p) * d) % p) % p, (b * c % p + a * d % p) % p };
    
    return result;
}

//b is the power here
static vector<int> cipolla_power_mod_p(vector<int>& term, int b, int fieldextension, int p) {
    vector<int> result = {1, 0};
    while (b > 0) {
        if (b % 2 == 1) {
            //result = (result * a) % p;
            result = cipollamult(result, term, fieldextension, p);
        }

        b = b >> 1;

        //a = (a * a) % p;
        term = cipollamult(term, term, fieldextension, p);
    }
    return result;
}

int squareroot(int n, int p) {
    n = ((n % p)+p) % p;
    if (power_mod_p(n, int((p-1)/2), p) != 1) {
        cout << "No square root exists" << endl;
        cout << "n = " << n << ", p = " << p << endl;
        return -15000000;
    }
    
    if (p % 4 == 3) {
        int sqrt = power_mod_p(n, int((p+1)/4), p); //neat trick: this is a solution to a^2 = b
        return sqrt;
    } else { //we need to apply cipolla's algo for x^2 = n mod p
        //step 1: find "a" such that a^2-n is not a square mod p
        int a = 0;
        int thingToEvaluate = 0;
        for (int i = 1; i < p; i++) {
            thingToEvaluate = (((power_mod_p(i, 2, p) - n) % p) + p) % p;
            if (power_mod_p(thingToEvaluate, int((p-1)/2), p) == p-1) { //NOT -1
                a = i;
                break;
            }
        }
        //step 2: compute thing in cipolla
        //we are adjoining a square root of a^2 - n, so we need to work in the field Z_p[sqrt(a^2-n)]
        int asquaredminusn = (((a*a - n) % p) + p) % p;
        vector<int> guyToRaisePowerTo = {a, 1};
        vector<int> result = cipolla_power_mod_p(guyToRaisePowerTo, (p+1)/2, asquaredminusn, p);
        
        return (result[0] % p + p) % p;
    }
}

// get A and B values from CSV file
int getAp(int p, int A, int B) {
    int c = 0;
    int repline = 0;

    vector<int> reps = findQuarticResidueClasses(p);

    string filename = "classdata/file_" + to_string(p) + ".csv";

    ifstream file;
    file.open(filename);
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
        int readThing = get_value_from_file(p, repline, B, file);

        file.close();
        return readThing;
    }

    int lfourth = A * inverse(c, p) % p;

    //step 2: calulcate lsixth
    int lsquare = squareroot(lfourth, p);
    //int lsquare = 1;
    int lsixth = lsquare * lfourth % p;

    //step 3: calculate B and lookup
    B = B * inverse(lsixth, p) % p;

    int readThing = get_value_from_file(p, repline, B, file);

    file.close();
    return readThing;

}




// Function to get value from a CSV file
int get_value_from_file(int prime, int repline, int b, ifstream &file) {

    string line;
    int target_line = prime * repline + b + 1; //+1 to account for header
    int current_line = 0;
    
    file.clear();           // Clear any error flags
    file.seekg(0, std::ios::beg); // Move the file pointer to the beginning

    while (getline(file, line)) {
        if (current_line == target_line) {
            stringstream ss(line);
            string item;
            int value;
            for (int i = 0; getline(ss, item, ','); ++i) {
                /*if (i == 0) {
                    cout << "rep = " << item << ", ";
                }

                if (i == 1) {
                    cout << "b = " << item << ", ";
                }*/

                if (i == 2) {
                    //cout << "value = " << item << "!";
                    value = stoi(item);
                    return value;
                }
            }
        }
        ++current_line;
    }
    return -1;
}

long largenummodp(vector<long>& digits, int prime) {
    long res = 0;
    int len = digits.size();
    for (int i=0; i < len; i++) {
        int temp = digits[i] % prime;
        int pow10 = power_mod_p(10, 4*i, prime);
        temp = (temp * pow10) % prime;
        res = (res + temp) % prime;
    }
    return res;
}

void generateSequences(const std::vector<int>& nums, int length, std::vector<int>& current, std::vector<std::vector<int>>& result) {
    if (length == 0) {
        result.push_back(current);
        return;
    }
    
    for (int num : nums) {
        current.push_back(num);
        generateSequences(nums, length - 1, current, result);
        current.pop_back(); // backtrack
    }
}

// Function to generate all possible sequences of a given length
std::vector<std::vector<int>> generateAllSequences(const std::vector<int>& nums, int length) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    generateSequences(nums, length, current, result);
    return result;
}
