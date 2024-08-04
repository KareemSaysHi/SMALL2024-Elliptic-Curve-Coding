#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

// Function to calculate a^b mod p without overflow
long power_mod_p(long a, long b, long p);

//calculate x over p
int legendre_symbol(long x, long p);

//find inverse of a mod p
long inverse(long a, long p);

int squareroot(int a, int p);

// Returs true if fourth power mod p, otherwise returns false
bool isFourthPower(long x, long p);

//finds quartic residue classes
vector<int> findQuarticResidueClasses(long p);

// Function to generate prime numbers up to k
vector<long> generate_primes_in_range(long lower, long upper);

int getAp(int p, int A, int B);

// Function to get value from a CSV file
int get_value_from_file(int prime, int repline, int b, ifstream &file);

long largenummodp(vector<long>& digits, int prime);

void generateSequences(const std::vector<int>& nums, int length, std::vector<int>& current, std::vector<std::vector<int>>& result);

vector<vector<int>> generateAllSequences(const std::vector<int>& nums, int length);