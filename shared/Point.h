#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>

template<int N> class Point : public std::vector<__int64> {
public:
    Point(std::initializer_list<__int64> il) : std::vector<__int64>(il) { resize(N); }
    Point() { resize(N); }
    Point(const Point& other) { resize(N); Assign(other); }

    const Point& operator=(const Point& other) { Assign(other); return *this; }
    void operator+=(const Point& other) { for (int c = 0; c < N; ++c) { at(c) += other.at(c); } }
    void operator-=(const Point& other) { for (int c = 0; c < N; ++c) { at(c) -= other.at(c); } }
    void operator*=(int x) { for (int c = 0; c < N; ++c) { at(c) *= x; } }
    void operator/=(int x) { for (int c = 0; c < N; ++c) { at(c) /= x; } }

    void Assign(const Point& other)
    {
        for (int i = 0; i < N; ++i) {
            at(i) = other.at(i);
        }
    }

    template<class _F>
    void DoNeighbours(_F fn) const
    {
        Point d;
        for (auto& x : d) x = -1;

        while (true) {
            if (!std::all_of(d.begin(), d.end(), [](__int64 a) {return a == 0; })) {
                fn((*this) + d);
            }
            
            int c = 0;
            while (c < N) {
                ++d[c];
                if (d[c] != 2) break;
                d[c] = -1;
                ++c;
            }
            if (c == N) break;
        }
    }

    template<class _F>
    void DoSquareNeighbours(_F fn) const
    {
        Point x(*this);
        for (int axis = 0; axis < N; ++axis)
        {
            --x.at(axis);
            fn(x);
            ++x.at(axis);
            ++x.at(axis);
            fn(x);
            --x.at(axis);
        }
    }
};

template<int N> Point<N> operator+ (const Point<N>& a, const Point<N>& b) { Point<N> p(a); p += b; return p; }
template<int N> Point<N> operator- (const Point<N>& a, const Point<N>& b) { Point<N> p(a); p -= b; return p; }
template<int N> Point<N> operator* (const Point<N>& a, int x) { Point<N> p(a); p *= x; return p; }
template<int N> Point<N> operator/ (const Point<N>& a, int x) { Point<N> p(a); p /= x; return p; }
template<int N> std::ostream& operator<< (std::ostream& os, const Point<N>& p)
{
    os << "(" << p.at(0);
    for (int i = 1; i < N; ++i) {
        os << ", " << p.at(i);
    }
    os << ")";

    return os;
}

// Manhattan distance
template<int N>
__int64 Dist(const Point<N>& p1, const Point<N>& p2) {
    __int64 sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += abs(p1[i] - p2[i]);
    }
    return sum;
}

typedef Point<2> Point2;
typedef Point<3> Point3;
typedef Point<4> Point4;

namespace P2
{
    const Point2 N({ 0, -1 });
    const Point2 S({ 0, 1 });
    const Point2 W({ -1, 0 });
    const Point2 E({ 1, 0 });

    const Point2 U({ 0, -1 });
    const Point2 D({ 0, 1 });
    const Point2 L({ -1, 0 });
    const Point2 R({ 1, 0 });

    std::map<char, Point2> DIRS(
        {
            {'N', N},
            {'S', S},
            {'W', W},
            {'E', E},
            {'n', N},
            {'s', S},
            {'w', W},
            {'e', E},
            {'U', U},
            {'D', D},
            {'L', L},
            {'R', R},
            {'u', U},
            {'d', D},
            {'l', L},
            {'r', R},
        }
    );
}