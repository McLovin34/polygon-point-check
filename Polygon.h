#pragma once

#include "Error.h"
#include "Point.h"
#include <vector>
#include <set> 

/// ����� Validator �������� �� ������������� ��������: ���������, �������, ��������, ������������ � �������� �������� �����.
class Validator {
public:
    Validator() = default;

    /// \brief ��������� ��� ��������� ������ (������ ������ + �������� �����).
    /// \param[in]   vertices  � ������ ������ ��������������.
    /// \param[in]   testPoint � �����, �������������� ������� �����������.
    /// \param[out]  err       � ������ Error, ���� ������������ �������� �� �������.
    /// \return true, ���� ������ ��������� �������; false � ��� ����������� ����� ������.
    bool validate(const std::vector<Point>& vertices, const Point& testPoint, Error& err);

private:
    // --- ��������������� ��������, ������� ������������ � validate � isValidPolygon ---

    /// �������� �� ���������� ������ (������ ���� [3, 1000])
    bool checkVertexCount(int vertexCount, Error& err);

    /// �������� �� ������������� �������
    bool checkDuplicateVertices(const std::vector<Point>& vertices, Error& err);

    /// ��������, ��� ������� ������ � ������� ������ (������ �������)
    bool checkCorrectVertexOrder(const std::vector<Point>& vertices, Error& err);

    /// �������� �������������� ��� �����
    bool checkCollinearity(const Point& a, const Point& b, const Point& c);

    /// �������� ����������� ���� �������� a1�a2 � b1�b2
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2);

    /// ��������, ��� x � y (������) ����� � ��������� [-999, 999]
    bool checkVertexRange(const Point& p, Error& err, int lineNumber);

    /// �������� �������� (���������� ���������������) � ������������
    bool checkPolygonShape(const std::vector<Point>& vertices, Error& err);

    /// ���������������: ���������� ��������������� ������� (���������) �� ������� ������
    long long signedArea(const std::vector<Point>& v);

    /// ���������������: ��������, ��� ����� q ����� �� ������� pr
    bool onSegment(const Point& p, const Point& q, const Point& r);

    /// ���������������: ���������� ���������� ������ ����� (p, q, r)
    /// \return 0 � �����������, 1 � �� �������, 2 � ������ �������
    int orientation(const Point& p, const Point& q, const Point& r);
};


