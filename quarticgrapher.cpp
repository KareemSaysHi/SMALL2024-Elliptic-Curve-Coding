#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h>
#include "helperfunctions.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <file_name> <upper_limit> <filter_on> <a> <mod b>" << endl;
        return 1;
    }

    int n = atoi(argv[1]); //how high do you want to go with your primes
    int filterOn = atoi(argv[2]); //turns filter on or not
    int filterA = atoi(argv[3]); //A
    int filterB = atoi(argv[4]); //modB

    
    std::vector<int> primes = generate_primes_in_range(0, n);

    vector<int> x; /*primes*/
    vector<double> y; /*A-values*/

    #pragma omp parallel for
    for (int p : primes) {

        if (filterOn) {
            if (p % filterB != filterA || p <= 5) {
                continue;
            }
        }

        int A = 0;
        int B = 0;
        int c = 0;
        int value = 0;
        int readThing = 0;
        int repline = 0;

        cout << "on prime " << p << std::endl;

        string filename = "classdata/file_" + to_string(p) + ".csv";

        //here we'll be looking at x^3 + A(T)x + B(T)
        
        vector<int> reps = findQuarticResidueClasses(p);

        for (int t = 0; t < p; ++t) {
            

            //define the function we're going to be looking at
            int t_cubed = (((t * t) % p) * t) % p;
            int t_squared = (t * t) % p;
            A = 1;
            B = t_squared % p; 

            /*int t_cubed = (((t * t) % p) * t) % p;
            A = 1;
            B = t_cubed;*/

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
                readThing = get_value_from_file(p, repline, B, filename);
                value += pow(readThing, 2); 
                continue;
            }

            int lfourth = A * inverse(c, p) % p;

            //step 2: calulcate lsixth
            int lsquare = squareroot(lfourth, p);
            //int lsquare = 1;
            int lsixth = lsquare * lfourth % p;

            //step 3: calculate B and lookup
            B = B * inverse(lsixth, p) % p;

            readThing = get_value_from_file(p, repline, B, filename);
            value += pow(readThing, 2);
        }
        cout << " p = " << p << ", sum = " << value << "\n";

        x.push_back(p);
        y.push_back((value - pow(p, 2)) / pow(p, 1.5)); //subtract by p^2 and divide by p^(3/2) to normalize
    }

    std::ofstream output_file("data.txt");
    if (!output_file.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < x.size(); ++i) {
        output_file << x[i] << "," << y[i] << std::endl;
    }

    return 0;
}