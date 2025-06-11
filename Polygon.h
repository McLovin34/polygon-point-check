#pragma once
#include "Error.h"
#include "Point.h"
#include <vector>

/// \brief Класс Polygon реализует невыпуклый многоугольник с методами проверки корректности и проверки принадлежности точки.
/// 
/// - Проверяет валидность (isValid): простота, невыпуклость, отсутствие коллинеарных троек.
/// - Проверяет принадлежность точки (contains): лежит ли точка внутри или на границе.
class Polygon {
public:
    /// Вектор вершин многоугольника (координаты уже должны быть валидированы по синтаксису)
    std::vector<Point> vertices;

    /// Конструктор по умолчанию (создаёт пустой многоугольник)
    Polygon() = default;

    /// Конструктор от вектора вершин
    /// \param v Вектор точек, определяющих вершины многоугольника
    explicit Polygon(const std::vector<Point>& v);

    /// \brief Проверяет корректность многоугольника.
    /// \details Многоугольник считается корректным, если он прост (нет самопересечений и дубликатов),
    ///          невыпуклый, без коллинеарных троек подряд, и вершины заданы в правильном порядке.
    /// \param[out] err Структура для описания найденной ошибки.
    /// \return true, если многоугольник валиден; false — если обнаружена ошибка.
    bool isValid(Error& err) const;

    /// \brief Проверяет, принадлежит ли точка p многоугольнику.
    /// \param[in] p Проверяемая точка.
    /// \return true, если точка лежит внутри или на границе; false — если вне многоугольника.
    bool contains(const Point& p) const;

///private:
    /// \brief Считает удвоенную ориентированную площадь многоугольника (по формуле Гаусса).
    /// \return Удвоенная площадь; знак определяет порядок обхода.
    long long signedArea() const;

    /// \brief Проверяет, лежат ли три точки на одной прямой (коллинеарны).
    /// \param a, b, c Проверяемые точки.
    /// \return true, если точки коллинеарны.
    bool checkCollinearity(const Point& a, const Point& b, const Point& c) const;

    /// \brief Проверяет, пересекаются ли два отрезка [a1,a2] и [b1,b2].
    /// \return true, если есть пересечение (или общая точка кроме концов).
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) const;

    /// \brief Проверяет, лежит ли точка q на отрезке pr.
    /// \return true, если точка q находится между p и r (включая концы).
    bool onSegment(const Point& p, const Point& q, const Point& r) const;

    /// \brief Определяет ориентацию тройки точек (p, q, r).
    /// \return 0 — коллинеарны, 1 — по часовой, 2 — против часовой стрелки.
    int orientation(const Point& p, const Point& q, const Point& r) const;

    /// \brief Проверка невыпуклости, отсутствия самопересечений и коллинеарных троек.
    /// \param[out] err Структура для описания найденной ошибки.
    /// \return true, если форма многоугольника валидна.
    bool checkPolygonShape(Error& err) const;
};


