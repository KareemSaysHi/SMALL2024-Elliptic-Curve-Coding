#include "helperfunctions.h"

long largenummodp(long digits[], int prime) {
    long res = 0;
    int len = *(&digits + 1) - digits;
    for (int i=0; i < len; i++) {
        int temp = digits[i] % prime;
        int pow10 = power_mod_p(10, 4*i, prime);
        temp = (temp * pow10) % prime;
        res = (res + temp) % prime;
    }
    return res;
}