#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

std::vector<std::string> Split(std::string s, char tok = ',')
{
    std::istringstream iss(s);
    std::vector<std::string> r;
    std::string n;

    while (getline(iss, n, tok)) {
        trim(n);
        r.push_back(n);
    }

    return r;
}

template<class _F>
void Split(std::string s, _F fn, char tok = ',')
{
    std::istringstream iss(s);
    std::string n;

    while (getline(iss, n, tok)) {
        trim(n);
        fn(n);
    }
}

std::vector<int> SplitInt(std::string s, char tok = ',')
{
    std::istringstream iss(s);
    std::vector<int> r;
    std::string n;

    while (getline(iss, n, tok)) {
        trim(n);
        r.push_back(stoi(n));
    }

    return r;
}

std::vector<__int64> SplitI64(std::string s, char tok = ',')
{
    std::istringstream iss(s);
    std::vector<__int64> r;
    std::string n;

    while (getline(iss, n, tok)) {
        trim(n);
        r.push_back(stoll(n));
    }

    return r;
}
