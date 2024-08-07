#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#inlcude<unordered_map>
#include <string>

using namespace std;

// Function to calculate a * b mod p without overflow
long multmodp(long a, long b, long mod);

// Function to calculate a^b mod p without overflow
long power_mod_p(long a, long b, long p);

//calculate x over p
int legendre_symbol(long x, long p);

//find inverse of a mod p
long inverse(long a, long p);

long squareroot(long a, long p);

// Returs true if fourth power mod p, otherwise returns false
bool isFourthPower(long x, long p);

//finds quartic residue classes
vector<int> findQuarticResidueClasses(long p);

// Function to generate prime numbers up to k
vector<long> generate_primes_in_range(long lower, long upper);

long largenummodp(vector<long>& digits, long prime);

void generateSequences(const std::vector<int>& nums, int length, std::vector<int>& current, std::vector<std::vector<int>>& result);

vector<vector<int>> generateAllSequences(const std::vector<int>& nums, int length);
