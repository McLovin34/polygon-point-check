#pragma once

#include <iostream>

/// ��������� ����� � float ������������
struct Point {
    float x;  // ���������� X (��� float ��� ��������� ������� ��������)
    float y;  // ���������� Y (��� float ��� ��������� ������� ��������)

    // �����������: ������������� ��������� (�� ��������� (0.0f, 0.0f))
    Point(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_) {}

    // �������� ��������� �� ��������� (������������ � set/map)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    // �������� "������" ��� �������� ����� � std::set ��� std::map
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};
