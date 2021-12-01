#include <iostream>

using namespace std;

int main()
{
    int numinc = 0;
#ifdef PART1
    int prevd = INT32_MAX;

    while (!cin.eof()) {
        int d;
        cin >> d;
        if (d > prevd) {
            ++numinc;
        }
        prevd = d;
    }

    cout << "Part 1 = " << numinc << endl;
#else
    int window = 0;
    int d1, d2, d3;
    cin >> d1;
    cin >> d2;
    cin >> d3;

    while (!cin.eof()) {
        int d4;
        cin >> d4;
        if (d1 + d2 + d3 < d2 + d3 + d4) {
            ++numinc;
        }
        d1 = d2;
        d2 = d3;
        d3 = d4;
    }

    cout << "Part 2 = " << numinc << endl;
#endif
}
