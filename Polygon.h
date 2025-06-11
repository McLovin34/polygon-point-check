#pragma once
#include "Error.h"
#include "Point.h"
#include <vector>

/// \brief ����� Polygon ��������� ���������� ������������� � �������� �������� ������������ � �������� �������������� �����.
/// 
/// - ��������� ���������� (isValid): ��������, ������������, ���������� ������������ �����.
/// - ��������� �������������� ����� (contains): ����� �� ����� ������ ��� �� �������.
class Polygon {
public:
    /// ������ ������ �������������� (���������� ��� ������ ���� ������������ �� ����������)
    std::vector<Point> vertices;

    /// ����������� �� ��������� (������ ������ �������������)
    Polygon() = default;

    /// ����������� �� ������� ������
    /// \param v ������ �����, ������������ ������� ��������������
    explicit Polygon(const std::vector<Point>& v);

    /// \brief ��������� ������������ ��������������.
    /// \details ������������� ��������� ����������, ���� �� ����� (��� ��������������� � ����������),
    ///          ����������, ��� ������������ ����� ������, � ������� ������ � ���������� �������.
    /// \param[out] err ��������� ��� �������� ��������� ������.
    /// \return true, ���� ������������� �������; false � ���� ���������� ������.
    bool isValid(Error& err) const;

    /// \brief ���������, ����������� �� ����� p ��������������.
    /// \param[in] p ����������� �����.
    /// \return true, ���� ����� ����� ������ ��� �� �������; false � ���� ��� ��������������.
    bool contains(const Point& p) const;

///private:
    /// \brief ������� ��������� ��������������� ������� �������������� (�� ������� ������).
    /// \return ��������� �������; ���� ���������� ������� ������.
    long long signedArea() const;

    /// \brief ���������, ����� �� ��� ����� �� ����� ������ (�����������).
    /// \param a, b, c ����������� �����.
    /// \return true, ���� ����� �����������.
    bool checkCollinearity(const Point& a, const Point& b, const Point& c) const;

    /// \brief ���������, ������������ �� ��� ������� [a1,a2] � [b1,b2].
    /// \return true, ���� ���� ����������� (��� ����� ����� ����� ������).
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) const;

    /// \brief ���������, ����� �� ����� q �� ������� pr.
    /// \return true, ���� ����� q ��������� ����� p � r (������� �����).
    bool onSegment(const Point& p, const Point& q, const Point& r) const;

    /// \brief ���������� ���������� ������ ����� (p, q, r).
    /// \return 0 � �����������, 1 � �� �������, 2 � ������ ������� �������.
    int orientation(const Point& p, const Point& q, const Point& r) const;

    /// \brief �������� ������������, ���������� ��������������� � ������������ �����.
    /// \param[out] err ��������� ��� �������� ��������� ������.
    /// \return true, ���� ����� �������������� �������.
    bool checkPolygonShape(Error& err) const;
};


