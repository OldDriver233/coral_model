#ifndef MISC_H
#define MISC_H
#include <concepts>

template<typename T>
concept is_number = std::is_integral_v<T> || std::is_floating_point_v<T>;

template<is_number T>
T clamp(T _val, T _min, T _max) {
    return _val < _min ? _min : (_val > _max ? _max : _val);
}

#endif //MISC_H
