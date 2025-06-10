#pragma once

#include <iostream>

/// ��������� ����� � ������ ������������
struct Point {
    int x;
    int y;

    Point(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}

    // ��������� ��� ��������� (����� ��� std::set ��� std::map)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

// ������� ������ ����� � ������� "(x;y)"
inline std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ";" << p.y << ")";
    return os;
}
