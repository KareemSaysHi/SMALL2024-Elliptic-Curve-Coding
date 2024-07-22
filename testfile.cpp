#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include "helperfunctions.h"

using namespace std;

void testResidueFunction(int n) {
    vector<int> quarticRes = findQuarticResidueClasses(n);
    cout << "Quartic Residue Classes for " << n << endl;
    for (int i = 0; i < quarticRes.size(); i++) {
        cout << quarticRes[i] << " ";
    }
    cout << endl;
}

void testSquareRoot(int n, int p) {
    int result = squareroot(n, p);
    cout << power_mod_p(n, int((p-1)/2), p) << endl;
    cout << "Square root of " << n << " mod " << p << " is " << result << endl;
}

/*void testCSVReader() {
    string filename = "classdata/file_13.csv";
    ifstream file(filename);
    string line;
    int repline = 3;
    int b = 0;
    get_value_from_file(13, repline, b, filename);
}*/

void testLargeNumModP() {
    vector<long> n = {1112, 9101, 5678, 123}; //written in reverse order
    int p = 13;
    cout << "Mod " << p << " is " << largenummodp(n, p) << endl;

}
// Main function
int main(int argc, char *argv[]) {
    testLargeNumModP();
    return 0;
}