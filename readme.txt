to compile:

g++ -fopenmp helperfunctions.cpp quarticclasses.cpp -o compute_frobenius_trace

g++ -fopenmp helperfunctions.cpp quarticgrapher.cpp -o compute_second_moment

This code computes a_p values for each of the (a priori) p^2 many elliptic curves mod p of the form y^2=x^3+ax+b.

It does this by finding quartic residue classes and preforming some automorphism. 
    - quarticclasses.cpp
It also calculates second moments of one-parameter families.
    - quarticgrapher.cpp
This can then be graphed. 
    - graphing.py


Authors:
    Timothy Cheek       (timcheek1729)
    Pico Gilman         (pg132)
    Kareem Jaber        (KareemSaysHi)
    Marie-Hélène Tomé   (m-ht3)
