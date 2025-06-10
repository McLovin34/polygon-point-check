#pragma once

#include "Error.h"
#include "Point.h"
#include <vector>

/// ����� Polygon ������������ ���������� ������������� � �����:
///  1) ��������� ���� ������������ (isValid),
///  2) ��������� �������������� ����� (contains).
class Polygon {
public:
    /// ������ ������, ��������������, ��� ��� ��� ��������� �� ������� ��������� � ���������
    std::vector<Point> vertices;

    Polygon() = default;
    explicit Polygon(const std::vector<Point>& v) : vertices(v) {}

    /// \brief ���������, ��� ������������� ��������� (�����, ��������, ������� ������ �������),
    ///        ��������� err, ���� ������� ������.
    /// \return true, ���� �� ��; false � � ��������� ������.
    bool isValid(Error& err) const;

    /// \brief ����������, ����������� �� ����� p ��������������.
    /// \param[in] p � ����������� �����.
    /// \return true, ���� ����� ������ ��� �� �������; false, ���� �������.
    bool contains(const Point& p) const;

private:
    // --- ��������������� �������������� ������, ������� �� ������ Polygon ---

    /// ��������, ��� ��� ����� �����������
    bool checkCollinearity(const Point& a, const Point& b, const Point& c) const;

    /// �������� �� ����������� ���� �������� a1�a2 � b1�b2
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) const;

    /// ���������� ��������������� ������� (���������) � ������� ������
    long long signedArea() const;

    /// ���������������: ��������, ��� ����� q ����� �� ������� pr
    bool onSegment(const Point& p, const Point& q, const Point& r) const;

    /// ���������������: ���������� ���������� ������ ����� (p, q, r)
    /// \return 0 � �����������, 1 � �� �������, 2 � ������ �������
    int orientation(const Point& p, const Point& q, const Point& r) const;

    /// �������� ������������ � ��������������� (����� isValid)
    bool checkPolygonShape(Error& err) const;
};


