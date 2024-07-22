#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

// Function to calculate a^b mod p without overflow
long power_mod_p(long a, int b, int p);

//calculate x over p
int legendre_symbol(int x, int p);

//find inverse of a mod p
int inverse(int a, int p);

int squareroot(int a, int p);

// Returs true if fourth power mod p, otherwise returns false
bool isFourthPower(int x, int p);

//finds quartic residue classes
vector<int> findQuarticResidueClasses(int p);

// Function to generate prime numbers up to k
vector<int> generate_primes_in_range(int lower, int upper);

int getAp(int p, int A, int B);

// Function to get value from a CSV file
int get_value_from_file(int prime, int repline, int b, ifstream &file);

long largenummodp(vector<long>& digits, int prime);