#pragma once
#include <algorithm>
#include "Point.h"

// Includes (start), but not (end).
// Assumes integral plane
template<int N>
class Rect {
private:
    Point<N> m_start;
    Point<N> m_end;

    static Point<N> ONE;
    static int PTS;

    void Normalise() {
        for (int i = 0; i < N; ++i) {
            if (m_start[i] > m_end[i]) {
                std::swap(m_start[i], m_end[i]);
                --m_start[i];
                ++m_end[i];
            }
        }
    }

public:
    Rect() {}
    Rect(const Point<N>& start, const Point<N>& end) :
        m_start(start), m_end(end)
    {
        Normalise();
    }
    static Rect Inclusive(const Point<N>& start, const Point<N>& end)
    {
        return Rect(start, end + ONE);
    }

    bool Contains(const Point<N>& p) const
    {
        for (int i = 0; i < N; ++i) {
            if (p[i] < m_start[i]) return false;
            if (p[i] >= m_end[i]) return false;
        }
        return true;
    }

    // Rect contains can line up ends
    bool Contains(const Rect<N>& r) const
    {
        for (int i = 0; i < N; ++i) {
            if (r.m_start[i] < m_start[i]) return false;
            if (r.m_end[i] > m_end[i]) return false;
        }
        return true;
    }

    const Point<N>& start() const
    {
        return m_start;
    }

    const Point<N>& end() const
    {
        return m_end;
    }

    const Point<N>& incend() const
    {
        return end - ONE;
    }

    Point<N> Length() const
    {
        return m_end - m_start;
    }

    __int64 Size() const
    {
        __int64 area = 1;
        for (int i = 0; i < N; ++i) {
            area *= (m_end[i] - m_start[i]);
        }

        return area;
    }

    bool operator==(const Rect<N>& other) const 
    {
        return (m_start == other.m_start && m_end == other.m_end);
    }

    bool operator<(const Rect<N>& other) const
    {
        if (m_start < other.m_start) return true;
        if (m_start > other.m_start) return false;
        return m_end < other.m_end;
    }

    Rect<N> Bound(const Rect<N>& other) const
    {
        Rect<N> ret;
        for (int i = 0; i < N; ++i) {
            ret.m_start[i] = std::min(m_start[i], other.m_start[i]);
            ret.m_end[i] = std::max(m_end[i], other.m_end[i]);
        }
        return ret;
    }

    __int64 BoundSize(const Rect<N>& other) const
    {
        __int64 area = 1;
        for (int i = 0; i < N; ++i) {
            area *= (std::max(m_end[i], other.m_end[i]) - std::min(m_start[i], other.m_start[i]));
        }

        return area;
    }

    Rect<N> Intersect(const Rect<N>& other) const
    {
        Rect<N> ret;
        for (int i = 0; i < N; ++i) {
            ret.m_start[i] = std::max(m_start[i], other.m_start[i]);
            ret.m_end[i] = std::max(ret.m_start[i], std::min(m_end[i], other.m_end[i]));
        }
        return ret;
    }

    bool Intersects(const Rect<N>& other) const
    {
        return Intersect(other).Size() > 0;
    }

    template<class _F>
    void DoCorners(_F fn) const
    {
        for (int i = 0; i < PTS; ++i) {
            Point<N> p;
            for (int j = 0; j < N; ++j) {
                if (i & (1 << j)) {
                    p[j] = m_end[j];
                }
                else {
                    p[j] = m_start[j];
                }

                fn(p);
            }
        }
    }
};

template<int N>
Rect<N> Bound(const Rect<N>& a, const Rect<N>& b)
{
    return a.Bound(b);
}

template<int N>
__int64 BoundSize(const Rect<N>& a, const Rect<N>& b)
{
    return a.BoundSize(b);
}

template<int N>
Rect<N> Intersect(const Rect<N>& a, const Rect<N>& b)
{
    return a.Intersect(b);
}

template<int N>
Rect<N> Intersects(const Rect<N>& a, const Rect<N>& b)
{
    return a.Intersects(b);
}

template<int N>
Point<N> Rect<N>::ONE = [] {
    Point<N> one;
    for (int i = 0; i < N; ++i) one[i] = 1;
    return one;
}();

template<int N>
int Rect<N>::PTS = 1 << N;

template<int N> std::ostream& operator<< (std::ostream& os, const Rect<N>& r)
{
    os << "[" << r.start() << "-" << r.end() << "]";
    return os;
}



typedef Rect<2> Rect2;
typedef Rect<3> Rect3;