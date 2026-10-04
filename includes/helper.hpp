#pragma once
#include <tuple>

struct Const
{
public:
    static const float PI;
};

std::pair<float, float> operator*(const std::pair<float, float> &left, float mul);
std::pair<float, float> operator/(const std::pair<float, float> &left, float divisor);