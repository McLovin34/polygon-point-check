#pragma once

#include "Error.h"
#include "Point.h"
#include <vector>
#include <set> 

/// Класс Validator отвечает за семантическую проверку: дубликаты, порядок, простота, невыпуклость и диапазон тестовой точки.
class Validator {
public:
    Validator() = default;

    /// \brief Проверяет уже считанные данные (вектор вершин + тестовая точка).
    /// \param[in]   vertices  – вектор вершин многоугольника.
    /// \param[in]   testPoint – точка, принадлежность которой проверяется.
    /// \param[out]  err       – объект Error, куда записываются сведения об ошибках.
    /// \return true, если данные полностью валидны; false — при обнаружении любой ошибки.
    bool validate(const std::vector<Point>& vertices, const Point& testPoint, Error& err);

private:
    // --- Вспомогательные проверки, которые используются в validate и isValidPolygon ---

    /// Проверка на количество вершин (должно быть [3, 1000])
    bool checkVertexCount(int vertexCount, Error& err);

    /// Проверка на дублирующиеся вершины
    bool checkDuplicateVertices(const std::vector<Point>& vertices, Error& err);

    /// Проверка, что вершины заданы в порядке обхода (против часовой)
    bool checkCorrectVertexOrder(const std::vector<Point>& vertices, Error& err);

    /// Проверка коллинеарности трёх точек
    bool checkCollinearity(const Point& a, const Point& b, const Point& c);

    /// Проверка пересечения двух отрезков a1–a2 и b1–b2
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2);

    /// Проверка, что x и y (вершин) лежат в диапазоне [-999, 999]
    bool checkVertexRange(const Point& p, Error& err, int lineNumber);

    /// Проверка простоты (отсутствие самопересечений) и невыпуклости
    bool checkPolygonShape(const std::vector<Point>& vertices, Error& err);

    /// Вспомогательная: вычисление ориентированной площади (удвоенной) по формуле Гаусса
    long long signedArea(const std::vector<Point>& v);

    /// Вспомогательная: проверка, что точка q лежит на отрезке pr
    bool onSegment(const Point& p, const Point& q, const Point& r);

    /// Вспомогательная: вычисление ориентации тройки точек (p, q, r)
    /// \return 0 — коллинеарны, 1 — по часовой, 2 — против часовой
    int orientation(const Point& p, const Point& q, const Point& r);
};


