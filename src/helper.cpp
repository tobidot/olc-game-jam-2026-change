#include "helper.hpp"

std::pair<float, float> operator*(const std::pair<float, float> &left, float mul)
{
    return std::make_pair(left.first * mul, left.second * mul);
}

std::pair<float, float> operator/(const std::pair<float, float> &left, float divisor)
{
    return std::make_pair(left.first / divisor, left.second / divisor);
}