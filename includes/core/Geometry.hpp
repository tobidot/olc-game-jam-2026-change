#pragma once
#include "olcPixelGameEngine3.h"

namespace geometry
{
template <typename T> struct Rect
{
    T top;
    T left;
    T bottom;
    T right;

    [[nodiscard]] olc::v_2d<T> TopLeft() const
    {
        return olc::v_2d<T>{top, left};
    }

    [[nodiscard]]
    olc::v_2d<T> TopRight() const
    {
        return olc::v_2d<T>{top, right};
    }

    [[nodiscard]]
    olc::v_2d<T> BottomLeft() const
    {
        return olc::v_2d<T>{bottom, left};
    }

    [[nodiscard]]
    olc::v_2d<T> BottomRight() const
    {
        return olc::v_2d<T>{bottom, right};
    }

    [[nodiscard]]
    bool Contains(olc::v_2d<T> point) const
    {
        return point.x >= left && point.x < right && point.y >= top && point.y < bottom;
    }
};

using RectF = Rect<float>;

} // namespace geometry