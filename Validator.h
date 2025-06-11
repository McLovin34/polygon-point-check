#pragma once
#include "Error.h"
#include "Point.h"
#include <vector>
#include <set>

/// \brief Класс Validator предназначен для полной проверки входных данных до построения Polygon.
/// 
/// Проверяет:
/// - Количество вершин и диапазон координат
/// - Отсутствие дубликатов вершин
/// - Правильный порядок обхода (ориентация)
/// - Простоту и невыпуклость многоугольника (набор геометрических проверок)
/// - Диапазон тестовой точки
class Validator {
public:
    /// Конструктор по умолчанию
    Validator() = default;

    /// \brief Главная функция проверки: вызывает все остальные проверки по шагам.
    /// \details Проверяет количество, уникальность, диапазон координат, порядок обхода,
    ///          простоту и невыпуклость многоугольника, диапазон тестовой точки.
    /// \param[in]  vertices  Вектор вершин многоугольника.
    /// \param[in]  testPoint Проверяемая точка.
    /// \param[out] err       Объект с описанием первой найденной ошибки.
    /// \return true, если все проверки пройдены; false — если есть ошибка.
    bool validate(const std::vector<Point>& vertices, const Point& testPoint, Error& err);


private:
    /// \brief Проверка количества вершин: [3, 1000].
    /// \param[in]  vertexCount Количество вершин.
    /// \param[out] err         Объект ошибки.
    /// \return true, если число допустимо.
    bool checkVertexCount(int vertexCount, Error& err);

    /// \brief Проверка на отсутствие дубликатов среди вершин.
    /// \param[in]  vertices Вектор вершин.
    /// \param[out] err      Объект ошибки.
    /// \return true, если дубликатов нет.
    bool checkDuplicateVertices(const std::vector<Point>& vertices, Error& err);

    /// \brief Проверка диапазона координат точки: [-999, 999] для обеих координат.
    /// \param[in]  p         Проверяемая точка.
    /// \param[out] err       Объект ошибки.
    /// \param[in]  lineNumber Номер строки для сообщения об ошибке.
    /// \return true, если точка в допустимых границах.
    bool checkVertexRange(const Point& p, Error& err, int lineNumber);

    /// \brief Проверка правильного порядка обхода вершин (ориентированной площади).
    /// \param[in]  vertices Вектор вершин.
    /// \param[out] err      Объект ошибки.
    /// \return true, если обход против часовой (площадь положительна).
    bool checkCorrectVertexOrder(const std::vector<Point>& vertices, Error& err);

    /// \brief Проверка простоты, невыпуклости и отсутствия коллинеарных троек.
    /// \param[in]  vertices Вектор вершин.
    /// \param[out] err      Объект ошибки.
    /// \return true, если форма корректна.
    bool checkPolygonShape(const std::vector<Point>& vertices, Error& err);

    // ===== Мини-геометрия для внутренних проверок (не для построения Polygon) =====

    /// \brief Вычисляет удвоенную ориентированную площадь по формуле Гаусса.
    /// \param[in] v Вектор вершин.
    /// \return Удвоенная площадь (может быть отрицательной).
    long long signedArea(const std::vector<Point>& v);

    /// \brief Проверка: коллинеарны ли три точки.
    /// \return true, если точки на одной прямой.
    bool checkCollinearity(const Point& a, const Point& b, const Point& c);

    /// \brief Проверка: пересекаются ли два отрезка.
    /// \return true, если отрезки имеют пересечение (не только по концам).
    bool checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2);

    /// \brief Проверка: лежит ли точка q на отрезке pr.
    /// \return true, если q строго между p и r (или совпадает с концом).
    bool onSegment(const Point& p, const Point& q, const Point& r);

    /// \brief Определяет ориентацию тройки точек (p, q, r).
    /// \return 0 — коллинеарны, 1 — по часовой, 2 — против часовой стрелки.
    int orientation(const Point& p, const Point& q, const Point& r);
    friend class Tests;
};
