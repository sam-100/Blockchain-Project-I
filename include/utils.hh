#ifndef UTILS_HH
#define UTILS_HH

#include <vector>
#include <ostream>
#include <unordered_set>
#include <list>


int random_int(int start, int end);
float random_float(float start, float end);
float random_exp_float(float mean);

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &arr) {
    out << "[ ";
    for(T item : arr)
        out << item << " ";
    out << "]";
    return out;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::unordered_set<T> &uset) {
    out << "{ ";
    for(T item : uset)
        out << item << " ";
    out << "}";
    return out;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::list<T> &l) {
    out << "{ ";
    for(T item : l)
        out << item << " ";
    out << "}";
    return out;
}

#endif