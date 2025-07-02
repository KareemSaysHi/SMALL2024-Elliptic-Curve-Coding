## Compilation

### Windows
1. Run the following: 
    ```
    g++ -fopenmp helperfunctions.cpp quarticclasses.cpp -o compute_frobenius_trace
    g++ -fopenmp helperfunctions.cpp quarticgrapher.cpp -o compute_second_moment
    ```

### Mac
1. Comment out `#include <omp.h>` for every `.cpp` file to disable multithreading. 
2. Run the following: 
    ```
    g++ helperfunctions.cpp quarticclasses.cpp -o compute_frobenius_trace
    g++ helperfunctions.cpp quarticgrapher.cpp -o compute_second_moment
    ```

## Usage 
The file `compute_frobenius_trace` generates the desired values of the trace of Frobenius for a range of primes `<lower>` to `<upper>`. Example:  
```
./compute_frobenius_trace 3 250000
```
We use this data for computing the biases. 

The file `compute_second_moment` takes 6+ parameters, in this order
- $n$: The upper bound on primes to check. 
- Turns filter on to only check primes congruent to $c \pmod d$. If $1$ it uses the filter, if $0$ it doesn't. 
- $c$
- $d$
- $\deg{A(T)}$
- $\deg{B(T)}$

Followed by 

- $\deg{A(T)}+1$ parameters, describing the coefficients of $A(T)$, where the ith parameter is the coefficient of $T^i$. 
- $\deg{B(T)}+1$ parameters, describing the coefficients of $B(T)$, where the ith parameter is the coefficient of $T^i$. 

This code computes $a_p$ values for each of the (a priori) $p^2$ many elliptic curves $\pmod{p}$ of the form $y^2=x^3+ax+b$.

It does this by finding quartic residue classes and performing some automorphism. 
`quarticclasses.cpp`

It also calculates second moments of one-parameter families.
`quarticgrapher.cpp`

This can then be graphed. 
`graphing.py`


## Authors:
- Timothy Cheek       (timcheek1729)
- Pico Gilman         (pg132)
- Kareem Jaber        (KareemSaysHi)
- Marie-Hélène Tomé   (m-ht3)
