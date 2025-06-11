#pragma once
#include "Error.h"
#include "Point.h"
#include <vector>
#include <set>

/// \brief ����� Validator ������������ ��� ������ �������� ������� ������ �� ���������� Polygon.
/// 
/// ���������:
/// - ���������� ������ � �������� ���������
/// - ���������� ���������� ������
/// - ���������� ������� ������ (����������)
/// - �������� � ������������ �������������� (����� �������������� ��������)
/// - �������� �������� �����
class Validator {
public:
    /// ����������� �� ���������
    Validator() = default;

    /// \brief ������� ������� ��������: �������� ��� ��������� �������� �� �����.
    /// \details ��������� ����������, ������������, �������� ���������, ������� ������,
    ///          �������� � ������������ ��������������, �������� �������� �����.
    /// \param[in]  vertices  ������ ������ ��������������.
    /// \param[in]  testPoint ����������� �����.
    /// \param[out] err       ������ � ��������� ������ ��������� ������.
    /// \return true, ���� ��� �������� ��������; false � ���� ���� ������.
    bool validate(const std::vector<Point>& vertices, const Point& testPoint, Error& err);


private:
    /// \brief �������� ���������� ������: [3, 1000].
    /// \param[in]  vertexCount ���������� ������.
    /// \param[out] err         ������ ������.
    /// \return true, ���� ����� ���������.
    bool checkVertexCount(int vertexCount, Error& err);

    /// \brief �������� �� ���������� ���������� ����� ������.
    /// \param[in]  vertices ������ ������.
    /// \param[out] err      ������ ������.
    /// \return true, ���� ���������� ���.
    bool checkDuplicateVertices(const std::vector<Point>& vertices, Error& err);

    /// \brief �������� ��������� ��������� �����: [-999, 999] ��� ����� ���������.
    /// \param[in]  p         ����������� �����.
    /// \param[out] err       ������ ������.
    /// \param[in]  lineNumber ����� ������ ��� ��������� �� ������.
    /// \return true, ���� ����� � ���������� ��������.
    bool checkVertexRange(const Point& p, Error& err, int lineNumber);

    /// \brief �������� ����������� ������� ������ ������ (��������������� �������).
    /// \param[in]  vertices ������ ������.
    /// \param[out] err      ������ ������.
    /// \return true, ���� ����� ������ ������� (������� ������������).
    bool checkCorrectVertexOrder(const std::vector<Point>& vertices, Error& err);

    /// \brief �������� ��������, ������������ � ���������� ������������ �����.
    /// \param[in]  vertices ������ ������.
    /// \param[out] err      ������ ������.
    /// \return true, ���� ����� ���������.
    bool checkPolygonShape(const std::vector<Point>& vertices, Error& err);

    // ===== ����-��������� ��� ���������� �������� (�� ��� ���������� Polygon) =====

    /// \brief ��������� ��������� ��������������� ������� �� ������� ������.
    /// \param[in] v ������ ������.
    /// \return ��������� ������� (����� ���� �������������).
    long long signedArea(const std::vector<Point>& v);

    /// \brief ��������: ����������� �� ��� �����.
    /// \return true, ���� ����� �� ����� ������.
    bool checkCollinearity(const Point& a, const Point& b, const Point& c);

    /// \brief ��������: ������������ �� ��� �������.
    /// \return true, ���� ������� ����� ����������� (�� ������ �� ������).
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2);

    /// \brief ��������: ����� �� ����� q �� ������� pr.
    /// \return true, ���� q ������ ����� p � r (��� ��������� � ������).
    bool onSegment(const Point& p, const Point& q, const Point& r);

    /// \brief ���������� ���������� ������ ����� (p, q, r).
    /// \return 0 � �����������, 1 � �� �������, 2 � ������ ������� �������.
    int orientation(const Point& p, const Point& q, const Point& r);
    friend class Tests;
};
