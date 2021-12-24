#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "../shared/Split.h"

using namespace std;

#if 0

struct State {
    map<string, __int64> regs;
    int pc;
    string inp;
    int inp_pos;
};

__int64 GetVal(string x, map<string, __int64>& regs)
{
    if (isalpha(x[0])) {
        return regs[x];
    }

    return stoll(x);
}

void Execute(string line, State& state)
{
    auto v = Split(line, ' ');
    if (v[0] == "inp") {
        state.regs[v[1]] = state.inp[state.inp_pos] - '0';
        ++state.inp_pos;
    }
    else if (v[0] == "add") {
        state.regs[v[1]] += GetVal(v[2], state.regs);
    }
    else if (v[0] == "mul") {
        state.regs[v[1]] *= GetVal(v[2], state.regs);
    }
    else if (v[0] == "div") {
        state.regs[v[1]] /= GetVal(v[2], state.regs);
    }
    else if (v[0] == "mod") {
        state.regs[v[1]] %= GetVal(v[2], state.regs);
    }
    else if (v[0] == "eql") {
        if (GetVal(v[1], state.regs) == GetVal(v[2], state.regs)) {
            state.regs[v[1]] = 1;
        }
        else {
            state.regs[v[1]] = 0;
        }
    }
    else {
        throw exception("Boom!");
    }
}

void Dump(const State& s)
{
    for (auto r : s.regs) {
        cout << r.first << "=" << r.second << " ";
    }
}
#endif

// Reverse Engineered round...
__int64 Round(__int64 inp, __int64 z, __int64 p1, __int64 p2, __int64 p3)
{
    __int64 x = z % 26 + p2;
    z /= p1;
    if (x != inp) {
        z *= 26;
        z += inp;
        z += p3;
    }

    return z;
}

int main()
{
#if 0
    vector<string> prog;

    string s;
    while (getline(cin, s)) {
        prog.push_back(s);
    }

    __int64 inp = 11111111111111;
    __int64 inc = 10000000000000;
    while (true) {
        stringstream ss;
        ss << inp;
        string digits = ss.str();

        //State state;
        //state.inp = ss.str();
        //state.pc = 0;
        //state.inp_pos = 0;

        //while (state.pc >= 0 && state.pc < prog.size()) {
        //    Execute(prog[state.pc], state);
        //    //cout << prog[state.pc] << "  ";
        //    //Dump(state);
        //    //cout << endl;
        //    ++state.pc;
        //}

        //cout << inp << ": " << state.regs["z"] << endl;
        __int64 z = 0;
        z = Round(digits[0] - '0', z, 1, 11, 16);
        z = Round(digits[1] - '0', z, 1, 12, 11);
        z = Round(digits[2] - '0', z, 1, 13, 12);
        z = Round(digits[3] - '0', z, 26, -5, 12);
        z = Round(digits[4] - '0', z, 26, -3, 12);
        z = Round(digits[5] - '0', z, 1, 14, 2);
        z = Round(digits[6] - '0', z, 1, 15, 11);
        z = Round(digits[7] - '0', z, 26, -16, 4);
        z = Round(digits[8] - '0', z, 1, 14, 12);
        z = Round(digits[9] - '0', z, 1, 15, 9);
        z = Round(digits[10] - '0', z, 26, -7, 10);
        z = Round(digits[11] - '0', z, 26, -11, 11);
        z = Round(digits[12] - '0', z, 26, -6, 6);
        z = Round(digits[13] - '0', z, 26, -11, 15);
        cout << "  " << z << endl;

        if (state.regs["z"] == 0) break;
        inp += inc;
    }
#endif

    vector<vector<__int64>> p = {
        {1, 11, 16},
        {1, 12, 11},
        {1, 13, 12},
        {26, -5, 12},
        {26, -3, 12},
        {1, 14, 2},
        {1, 15, 11},
        {26, -16, 4},
        {1, 14, 12},
        {1, 15, 9},
        {26, -7, 10},
        {26, -11, 11},
        {26, -6, 6},
        {26, -11, 15},
    };
    
    string part1;
    string part2;

    for (__int64 x = 9999999; x >= 1111111; --x) {
        stringstream ss;
        ss << x;
        string digits = ss.str();
        if (digits.find('0') != string::npos) continue;
        ss.str("");

        int digpos = 0;
        __int64 z = 0;
        for (int round = 0; round < 14; ++round) {

            // From algebra, the ones with a 26 in the first param
            // must have the digit as (z mod 26 + second param).  I
            // verified that in Excel.
            __int64 n;
            if (p[round][0] == 26) {
                n = z % 26 + p[round][1];
                if (n < 0) n += 26;
                if (n <= 0 || n > 9) {
                    z = -1;
                    break;
                }
            }
            else {
                n = digits[digpos] - '0';
                ++digpos;
            }

            ss << n;
            z = Round(n, z, p[round][0], p[round][1], p[round][2]);
        }

        if (z == 0) {
            if (part1.empty()) part1 = ss.str();
            part2 = ss.str();
        }
    }
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}