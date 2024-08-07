to compile:

g++ -fopenmp helperfunctions.cpp quarticclasses.cpp -o computegarbage

g++ -fopenmp helperfunctions.cpp quarticgrapher.cpp -o graphgarbage

This code computes a_p values for each of the (apriori) p^2 many elliptic curves mod p of the form y^2=x^3+ax+b.
It does this by finding quartic residue classes and preforming some automorphism. It also calculates second moments of one-parameter families.
