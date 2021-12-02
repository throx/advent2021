#include <iostream>
#include <string>

using namespace std;

int main()
{
    int x = 0;
    int dep = 0;
    int dep2 = 0;

    while (!cin.eof()) {
        string s;
        int d;
        cin >> s >> d;

        if (s == "forward"s) {
            x += d;
            dep2 += (d * dep);      // Turns out "aim" is exactly the same thing as "depth" from part 1
        }
        else if (s == "down"s) {
            dep += d;
        }
        else if (s == "up"s) {
            dep -= d;
        }
        else {
            throw "boom";
        }
    }

    cout << "Part 1 = " << x * dep << endl;
    cout << "Part 2 = " << x * dep2 << endl;
}
