#pragma once

#include <string>


template<typename T>
class Edge;

inline bool compareIntIncrease(const int &p1, const int &p2) { // Используется
    return p1 < p2;
}

inline bool compareIntDecrease(const int &p1, const int &p2) {
    return p1 > p2;
}

inline bool compareFloatIncrease(const float &p1, const float &p2) { // Используется
    return p1 < p2;
}

inline bool compareFloatDecrease(const float &p1, const float &p2) {
    return p1 > p2;
}

inline bool compareStringIncrease(const std::string &p1, const std::string &p2) { // Используется
    return p1 < p2;
}

inline bool compareStringDecrease(const std::string &p1, const std::string &p2) {
    return p1 > p2;
}