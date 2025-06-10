#pragma once

#include <iostream>

/// Структура точки с float координатами
struct Point {
    float x;  // Изменено на float
    float y;  // Изменено на float

    Point(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_) {}

    // Операторы для сравнения (нужно для std::set или std::map)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};
