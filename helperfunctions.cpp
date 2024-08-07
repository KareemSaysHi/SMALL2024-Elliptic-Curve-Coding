#include "helperfunctions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//https://www.geeksforgeeks.org/multiply-large-integers-under-large-modulo/
//algorithm is log(max(a,b)), so don't want to run often 
long multmodp(long a, long b, long mod) { 
    if (a*b<=0) {//is negative if it overflows, and only want to run slow algorithm if we're in this situation
        /*long res = 0; // Initialize result 
  
        // Update a if it is more than 
        // or equal to mod 
        a %= mod; 
  
        while (b) { 
            // If b is odd, add a with result 
            if (b & 1) 
                res = (res + a) % mod; 
  
            // Here we assume that doing 2*a 
            // doesn't cause overflow 
            a = (2 * a) % mod; 
  
            b >>= 1; // b = b / 2 
        } 
  
        return (res+mod)%mod;
        */
       long long arcst = (long long) a;
       long long brcst = (long long) b;
       long long result = arcst * brcst;
       if (result % mod < 0) return result % mod;
       else return (result % mod + mod) % mod;
    }

    return a*b % mod;
} 

// Function to calculate a^b mod p without overflow
long power_mod_p(long a, long b, long p) {
    //long long a = (long long) a; //Cast a to long long data type to avoid overflow when squaring
    long result = 1;
    while (b > 0) {
        if (b % 2 == 1) {
            result = multmodp(result, a, p);
        }

        b = b >> 1;
        a = multmodp(a, a, p);
    }
    return ((result % p) + p) % p;
}

// Function to calculate Legendre symbol
int legendre_symbol(long x, long p) {
    if (x % p == 0) {
        return 0;
    } else if (power_mod_p(x, long((p-1)/2), p) == 1) {
        return 1;
    } else {
        return -1;
    }
}

//find inverse of a mod p
long inverse(long a, long p) {
    return power_mod_p(a, p-2, p);
}

// Returns true if fourth power mod p, otherwise returns false
bool isFourthPower(long x, long p) {
    if (p % 4 == 1) {
        if (power_mod_p(x, long((p-1)/4), p) == 1) { //1 iff x is a fourth power mod p since multiplicative group is cyclic
            return true;
        }
    } else {
        if (power_mod_p(x, long((p-1)/2), p) == 1) { //in 3 mod 4, if you're a square, you're a fourth power!
            return true;
        }
    }
    return false;
}

vector<int> findQuarticResidueClasses(long p) {
    //need to make this work for p = 2, 3, maybe 5
    if (p == 2) {
        return vector<int>{0, 1};
    } else if (p == 3) {
        return vector<int>{0, 1, 2};
    } else if (p == 5) {
        return vector<int>{0, 1, 2, 3, 4}; //all in different
    }
    long thingToCheck = 0;
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
vector<long> generate_primes_in_range(long lower, long upper) {
    vector<long> primes;
    for (long num = 2; num < upper; num++) {
        bool is_prime = true;
        for (long i = 2; i <= sqrt(num); i++) {
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

static vector<long> cipollamult(vector<long>& term1, vector<long>& term2, long fieldextension, long p) {
    // Ensure the input vectors are of size 2
    if (term1.size() != 2 || term2.size() != 2) {
        throw std::invalid_argument("Both input vectors must be of size 2.");
    }
    
    long a = term1[0];
    long b = term1[1];
    long c = term2[0];
    long d = term2[1];

    //vector<long> result = {(a * c % p + (((fieldextension * b) % p) * d) % p) % p, (b * c % p + a * d % p) % p };
    vector<long> result = {(multmodp(a, c, p) + multmodp(multmodp(fieldextension, b, p), d, p))%p, (multmodp(b, c, p) + multmodp(a, d, p))%p };
    return result;
}

//b is the power here
static vector<long> cipolla_power_mod_p(vector<long>& term, long b, long fieldextension, long p) {
    vector<long> result = {1, 0};
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

long squareroot(long n, long p) {
    n = ((n % p)+p) % p;
    if (power_mod_p(n, long((p-1)/2), p) != 1) {
        cout << "No square root exists" << endl;
        cout << "n = " << n << ", p = " << p << endl;
        return -15000000;
    }
    
    if (p % 4 == 3) {
        long sqrt = power_mod_p(n, long((p+1)/4), p); //neat trick: this is a solution to a^2 = b
        return sqrt;
    } else { //we need to apply cipolla's algo for x^2 = n mod p
        //step 1: find "a" such that a^2-n is not a square mod p
        long a = 0;
        long thingToEvaluate = 0;
        for (long i = 1; i < p; i++) {
            thingToEvaluate = (((power_mod_p(i, 2, p) - n) % p) + p) % p;
            if (power_mod_p(thingToEvaluate, long((p-1)/2), p) == p-1) { //NOT -1
                a = i;
                break;
            }
        }
        //step 2: compute thing in cipolla
        //we are adjoining a square root of a^2 - n, so we need to work in the field Z_p[sqrt(a^2-n)]
        long asquaredminusn = (((multmodp(a, a, p) - n) % p) + p) % p;
        vector<long> guyToRaisePowerTo = {a, 1};
        vector<long> result = cipolla_power_mod_p(guyToRaisePowerTo, long((p+1)/2), asquaredminusn, p);
        
        return (result[0] % p + p) % p;
    }
}

long largenummodp(vector<long>& digits, int prime) {
    long res = 0;
    int len = digits.size();
    for (int i=0; i < len; i++) {
        int temp = digits[i] % prime;
        long pow10 = power_mod_p(10, 4*i, prime);
        temp = multmodp(temp, pow10, prime);
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
