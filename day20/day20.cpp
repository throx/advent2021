#include <iostream>
#include <string>
#include "../shared/Point.h"
#include <vector>
#include <set>
#include "../shared/Console.h"

using namespace std;

typedef vector<string> Pix;
const int MAXITER = 50;

int HashCount(const Pix& pix)
{
    int ret = 0;

    for (auto s : pix) {
        for (auto c : s) {
            if (c == '#') {
                ++ret;
            }
        }
    }

    return ret;
}


int main()
{
    vector<bool> key;
    Pix pix;

    string prepend(MAXITER + 5, '.');

    string s;
    getline(cin, s);
    for (auto c : s) {
        key.push_back(c == '#');
    }

    while (getline(cin, s)) {
        if (s.empty()) continue;
        pix.push_back(s);
    }

    int xsz = prepend.length() * 2 + pix[0].length();
    int ysz = prepend.length() * 2 + pix.size();

    Pix npix;
    for (int i = 0; i < prepend.length(); ++i) npix.push_back(prepend + string(pix[0].size(), '.') + prepend);
    for (string s : pix) npix.push_back(prepend + s + prepend);
    for (int i = 0; i < prepend.length(); ++i) npix.push_back(prepend + string(pix[0].size(), '.') + prepend);
    swap(pix, npix);

    int part1 = 0;

    for (int iter = 0; iter < MAXITER; ++iter) {
        npix.clear();
        for (int y = 0; y < ysz; ++y) {
            string line(xsz, '.');
            for (int x = 0; x < xsz; ++x) {
                int lookup = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        // Edge is infinitely repeated
                        int xx = min(max(0, x + dx), xsz - 1);
                        int yy = min(max(0, y + dy), ysz - 1);
                        if (pix[yy][xx] == '#') {
                            int pwr = (1 - dx) + (1 - dy) * 3;
                            lookup |= (1 << pwr);
                        }
                    }
                }

                if (key[lookup]) line[x] = '#';
            }
            npix.push_back(line);
        }

        swap(pix, npix);
        if (iter == 1) {
            part1 = HashCount(pix);
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << HashCount(pix) << endl;
}