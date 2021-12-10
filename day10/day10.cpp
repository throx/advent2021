#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<string> inp;

    while (!cin.eof()) {
        string s;
        cin >> s;

        inp.push_back(s);
    }

    vector<__int64> scores2;
    int part1 = 0;
    for (auto s : inp) {
        char e = 0;
        list<char> stack;
        for (char c : s) {
            switch (c) {
                case '(' :
                case '{' :
                case '[' :
                case '<' :
                    stack.push_back(c);
                    break;

                case ')':
                    if (stack.back() != '(') {
                        e = c;
                        goto done;
                    }
                    stack.pop_back();
                    break;

                case ']':
                    if (stack.back() != '[') {
                        e = c;
                        goto done;
                    }
                    stack.pop_back();
                    break;

                case '}':
                    if (stack.back() != '{') {
                        e = c;
                        goto done;
                    }
                    stack.pop_back();
                    break;

                case '>':
                    if (stack.back() != '<') {
                        e = c;
                        goto done;
                    }
                    stack.pop_back();
                    break;
            }
        }

    done:
        switch (e) {
        case ')':
            part1 += 3;
            break;
        case ']':
            part1 += 57;
            break;
        case '}':
            part1 += 1197;
            break;
        case '>':
            part1 += 25137;
            break;
        default:
            {
                __int64 score = 0;
                while (!stack.empty()) {
                    char x = stack.back();
                    stack.pop_back();
                    score *= 5;

                    switch (x) {
                    case '(':
                        score += 1;
                        break;
                    case '[':
                        score += 2;
                        break;
                    case '{':
                        score += 3;
                        break;
                    case '<':
                        score += 4;
                        break;
                    }
                }
                scores2.push_back(score);
            }
            
            break;
        }
    }

    sort(scores2.begin(), scores2.end());
    __int64 part2 = scores2[scores2.size() / 2];

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}