#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> Partition(const vector<string>& data, int bitpos, bool keephigh)
{
    int c = 0;
    vector<string> ones;
    vector<string> zeros;
    for (auto& s : data) {
        if (s[bitpos] == '1') {
            ones.push_back(s);
        }
        else {
            zeros.push_back(s);
        }
    }

    if (ones.size() >= zeros.size()) {
        return keephigh ? ones : zeros;
    }

    return keephigh ? zeros : ones;
}

int main()
{
    vector<int> counts;
    vector<string> nums;

    while (!cin.eof()) {
        string s;
        cin >> s;
        nums.push_back(s);

        if (counts.empty()) {
            counts.resize(s.length());
        }

        for (int i = 0; i < s.length(); ++i) {
            if (s[i] == '1') {
                ++counts[i];
            }
        }
    }

    int ep = 0;
    int del = 0;
    int n = nums.size();

    for (auto x : counts) {
        ep *= 2;
        del *= 2;
        if (x > (n / 2)) {
            ++ep;
        }
        else {
            ++del;
        }
    }

    cout << "Part 1: " << ep * del << endl;

    // Oxy
    vector<string> data = nums;
    int bitpos = 0;
    while (data.size() > 1) {
        data = Partition(data, bitpos, true);
        ++bitpos;
    }

    int oxy = stoi(data[0], nullptr, 2);


    // CO2
    data = nums;
    bitpos = 0;
    while (data.size() > 1) {
        data = Partition(data, bitpos, false);
        ++bitpos;
    }

    int co2 = stoi(data[0], nullptr, 2);

    cout << "Part 2: " << oxy * co2 << endl;
}
