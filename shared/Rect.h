#pragma once
#include <algorithm>
#include "Point.h"

// Includes (start), but not (end).
// Assumes integral plane
class Rect {
private:
    Point2 m_start;
    Point2 m_end;

    void Normalise() {
        if (m_start[0] > m_end[0]) {
            std::swap(m_start[0], m_end[0]);
            --m_start[0];
            ++m_end[0];
        }
        if (m_start[1] > m_end[1]) {
            std::swap(m_start[1], m_end[1]);
            --m_start[1];
            ++m_end[1];
        }
    }
public:
    Rect() {}
    Rect(const Point2& start, const Point2& end) :
        m_start(start), m_end(end)
    {
        Normalise();
    }
    static Rect Inclusive(const Point2& start, const Point2& end)
    {
        return Rect(start, end + Point2({ 1, 1 }));
    }

    bool Contains(const Point2& p) const {
        return p[0] >= m_start[0] &&
            p[0] < m_end[0] &&
            p[1] >= m_start[1] &&
            p[1] < m_end[1];
    }

    const Point2& start() const
    {
        return m_start;
    }

    const Point2& end() const
    {
        return m_end;
    }
};