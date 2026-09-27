#pragma once
#include "olc/olcPixelGameEngine3.h"

namespace core
{

struct Vector : public olc::vf2d
{
    using olc::vf2d::vf2d;

    explicit Vector(const olc::vf2d &cpy)
    {
        x = cpy.x;
        y = cpy.y;
    }

    explicit Vector(const olc::vi2d &cpy)
    {
        x = static_cast<float>(cpy.x);
        y = static_cast<float>(cpy.y);
    }

    Vector mul(const Vector &other)
    {
        return {x * other.x, y * other.y};
    }

    Vector mul(const olc::vf2d &other)
    {
        return {x * other.x, y * other.y};
    }
};

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
    float Height() const
    {
        return bottom - top;
    }

    [[nodiscard]]
    float Width() const
    {
        return right - left;
    }

    [[nodiscard]]
    bool Contains(olc::v_2d<T> point) const
    {
        return point.x >= left && point.x < right && point.y >= top && point.y < bottom;
    }
};

struct Polygon
{
    std::vector<Vector> points;
};

using RectF = Rect<float>;

} // namespace core