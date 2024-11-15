#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include "quaternion.hpp"

template <typename T>
std::vector<T> createUniformTimeseries(std::vector<T> quatsRaw, double samplingInterval, double horizonLength);

#endif /* UTIL_HPP */