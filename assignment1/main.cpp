#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "perfect.h"
#include "soundex.h"
using namespace std;


int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    findPerfects(40000);
    // Comment out the above line and uncomment below line 
    // to switch between running perfect.cpp and soundex.cpp
//    soundexSearch("res/surnames.txt");

    cout << endl << "main() completed." << endl;
    return 0;
}
