#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Node {
    bool isnum;
    int num;
};

typedef vector<Node> Num;

string ToString(const Num& num)
{
    stringstream ss;
    for (auto& x : num) {
        if (x.isnum) {
            ss << x.num;
        }
        else {
            ss << (char)x.num;
        }
    }

    return ss.str();
}

void Add(Num& num, const Num& num2)
{
    num.insert(num.begin(), { false, '[' });
    num.push_back({ false, ',' });
    for (auto& n : num2) {
        num.push_back(n);
    }
    num.push_back({ false, ']' });
}

bool ExplodeOne(Num& num)
{
    // Find bracket inside 4 pairs
    int bracketcount = 0;
    for (auto it = num.begin(); it != num.end(); ++it) {
        if (!it->isnum) {
            if (it->num == '[') {
                ++bracketcount;
                if (bracketcount > 4)
                {
                    if ((it + 1)->isnum &&
                        !(it + 2)->isnum && (it + 2)->num == ',' &&
                        (it + 3)->isnum &&
                        !(it + 4)->isnum && (it + 4)->num == ']') {

                        // Found a regular pair inside 4 brackets
                        int n1 = (it + 1)->num;
                        int n2 = (it + 3)->num;

                        // Scan backwards
                        for (auto it2 = it; it2 != num.begin(); --it2) {
                            if (it2->isnum) {
                                it2->num += n1;
                                break;
                            }
                        }

                        // Scan forwards
                        for (auto it2 = (it+5); it2 != num.end(); ++it2) {
                            if (it2->isnum) {
                                it2->num += n2;
                                break;
                            }
                        }

                        // Erase regular pair (leave last paren for replace)
                        it = num.erase(it, it + 4);

                        it->isnum = true;
                        it->num = 0;
                        return true;
                    }
                }
            }
            else if (it->num == ']') {
                --bracketcount;
            }
        }
    }

    return false;
}

bool SplitOne(Num& num)
{
    for (auto it = num.begin(); it != num.end(); ++it) {
        if (it->isnum && it->num > 9) {
            int n = it->num;
            it = num.erase(it);
            num.insert(it, { {false, '['}, {true, n / 2}, {false, ','}, {true, n / 2 + n % 2}, {false, ']'} });
            return true;
        }
    }
    return false;
}

void Reduce(Num& num)
{
//    cout << ToString(num) << endl;
    while (true) {
        if (!ExplodeOne(num)) {
            if (!SplitOne(num)) {
                break;
            }
        }
//        cout << ToString(num) << endl;
    }
}

int Mag(Num num)
{
    bool loop = true;
    while (loop && num.size() > 4) {
        loop = false;
        for (auto it = num.begin(); it+4 != num.end(); ++it) {
            if (!it->isnum && it->num == '[' &&
                (it + 1)->isnum &&
                !(it + 2)->isnum && (it + 2)->num == ',' &&
                (it + 3)->isnum &&
                !(it + 4)->isnum && (it + 4)->num == ']') {

                // Got a pair, collapse it
                int n1 = (it + 1)->num;
                int n2 = (it + 3)->num;
                it = num.erase(it, it + 4);
                it->isnum = true;
                it->num = 3 * n1 + 2 * n2;

                loop = true;
                break;
            }
        }
    }

    if (num.size() != 1) {
        throw exception("Boom");
    }
    return num[0].num;
}

int SumAll(const vector<Num>& nums)
{
    Num acc;
    for (auto num : nums) {
        if (!acc.empty()) {
            Add(acc, num);
            Reduce(acc);
        }
        else {
            swap(acc, num);
        }
    }

    return Mag(acc);
}

int main()
{
    vector<Num> nums;

    string s;
    while (getline(cin, s)) {
        Num num;
        for (char x : s) {
            Node n;
            if (isdigit(x)) {
                n.num = x - '0';
                n.isnum = true;
            }
            else {
                n.num = x;
                n.isnum = false;
            }

            num.push_back(n);
        }

        nums.push_back(num);
    }


    cout << "Part 1: " << SumAll(nums) << endl;

    int part2 = 0;
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = 0; j < nums.size(); ++j) {
            if (i == j) continue;
            part2 = max(part2, SumAll({ nums[i], nums[j] }));
        }
    }

    cout << "Part 2: " << part2 << endl;
}