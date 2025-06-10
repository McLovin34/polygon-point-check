#pragma once

#include "Error.h"
#include "Point.h"
#include <vector>

/// Класс Polygon представляет невыпуклый многоугольник и умеет:
///  1) проверить свою корректность (isValid),
///  2) проверить принадлежность точки (contains).
class Polygon {
public:
    /// Вектор вершин, предполагается, что они уже проверены на базовый синтаксис и диапазоны
    std::vector<Point> vertices;

    Polygon() = default;
    explicit Polygon(const std::vector<Point>& v) : vertices(v) {}

    /// \brief Проверяет, что многоугольник корректен (прост, невыпукл, порядок против часовой),
    ///        заполняет err, если найдена ошибка.
    /// \return true, если всё ок; false — в противном случае.
    bool isValid(Error& err) const;

    /// \brief Определяет, принадлежит ли точка p многоугольнику.
    /// \param[in] p – проверяемая точка.
    /// \return true, если точка внутри или на границе; false, если снаружи.
    bool contains(const Point& p) const;

private:
    // --- Вспомогательные геометрические методы, зависят от класса Polygon ---

    /// Проверка, что три точки коллинеарны
    bool checkCollinearity(const Point& a, const Point& b, const Point& c) const;

    /// Проверка на пересечение двух отрезков a1–a2 и b1–b2
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) const;

    /// Вычисление ориентированной площади (удвоенной) – формула Гаусса
    long long signedArea() const;

    /// Вспомогательная: проверка, что точка q лежит на отрезке pr
    bool onSegment(const Point& p, const Point& q, const Point& r) const;

    /// Вспомогательная: вычисление ориентации тройки точек (p, q, r)
    /// \return 0 — коллинеарны, 1 — по часовой, 2 — против часовой
    int orientation(const Point& p, const Point& q, const Point& r) const;

    /// Проверка невыпуклости и самопересечений (часть isValid)
    bool checkPolygonShape(Error& err) const;
};


