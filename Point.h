#pragma once

#include <iostream>

/// Структура точки с float координатами
struct Point {
    float x;  // Координата X (тип float для поддержки дробных значений)
    float y;  // Координата Y (тип float для поддержки дробных значений)

    // Конструктор: инициализация координат (по умолчанию (0.0f, 0.0f))
    Point(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_) {}

    // Оператор сравнения на равенство (используется в set/map)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    // Оператор "меньше" для хранения точек в std::set или std::map
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};
