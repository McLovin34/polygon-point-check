#include "Validator.h"
#include <set>
#include <climits>
#include <set> 

// private: вычисление ориентированной площади (удвоенной) — формула Гаусса
long long Validator::signedArea(const std::vector<Point>& v) {
    long long area2 = 0;
    int n = (int)v.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area2 += (long long)v[i].x * v[j].y - (long long)v[j].x * v[i].y;
    }
    return area2;
}

// private: определение ориентации трёх точек (p, q, r)
int Validator::orientation(const Point& p, const Point& q, const Point& r) {
    long long val = (long long)(q.y - p.y) * (r.x - q.x) - (long long)(q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

// private: проверка, что точка q лежит на отрезке pr
bool Validator::onSegment(const Point& p, const Point& q, const Point& r) {
    return (std::min(p.x, r.x) <= q.x && q.x <= std::max(p.x, r.x) &&
        std::min(p.y, r.y) <= q.y && q.y <= std::max(p.y, r.y));
}

bool Validator::checkVertexCount(int vertexCount, Error& err) {
    if (vertexCount < 3 || vertexCount > 1000) {
        err.type = ErrorType::invalidVertexCount;
        err.errorLineNumber = 1;
        err.errorLineContent = std::to_string(vertexCount);
        err.errorMessage = "Некорректное количество вершин. Допустимый диапазон: [3, 1000].";
        return false;
    }
    return true;
}

bool Validator::checkDuplicateVertices(const std::vector<Point>& vertices, Error& err) {
    std::set<Point> seen;
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (seen.find(vertices[i]) != seen.end()) {
            err.type = ErrorType::duplicateVertex;
            err.errorLineNumber = static_cast<int>(i + 2); // +2: 1-я строка — N, далее строки с вершинами начинаются с 2
            err.errorLineContent = std::to_string(vertices[i].x) + ";" + std::to_string(vertices[i].y);
            err.errorMessage = "Входные данные содержат дублирующиеся вершины: (" +
                std::to_string(vertices[i].x) + ";" + std::to_string(vertices[i].y) + ").";
            return false;
        }
        seen.insert(vertices[i]);
    }
    return true;
}

bool Validator::checkCorrectVertexOrder(const std::vector<Point>& vertices, Error& err) {
    long long area2 = signedArea(vertices);
    // Против часовой (стандарт для положительной ориентированной площади)
    if (area2 <= 0) {
        err.type = ErrorType::wrongOrder;
        err.errorLineNumber = 0;
        err.errorLineContent = "";
        err.errorMessage = "Вершины многоугольника заданы не в порядке обхода. Упорядочьте их корректно.";
        return false;
    }
    return true;
}

bool Validator::checkCollinearity(const Point& a, const Point& b, const Point& c) {
    long long cross = (long long)(b.x - a.x) * (c.y - a.y) - (long long)(b.y - a.y) * (c.x - a.x);
    return (cross == 0);
}

bool Validator::checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) {
    int o1 = orientation(a1, a2, b1);
    int o2 = orientation(a1, a2, b2);
    int o3 = orientation(b1, b2, a1);
    int o4 = orientation(b1, b2, a2);

    // Общий случай
    if (o1 != o2 && o3 != o4) return true;

    // Специальные случаи (коллинеарность)
    if (o1 == 0 && onSegment(a1, b1, a2)) return true;
    if (o2 == 0 && onSegment(a1, b2, a2)) return true;
    if (o3 == 0 && onSegment(b1, a1, b2)) return true;
    if (o4 == 0 && onSegment(b1, a2, b2)) return true;

    return false;
}

bool Validator::checkVertexRange(const Point& p, Error& err, int lineNumber) {
    if (p.x < -999 || p.x > 999 || p.y < -999 || p.y > 999) {
        err.type = ErrorType::coordinateOutOfRange;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = std::to_string(p.x) + ";" + std::to_string(p.y);
        err.errorMessage = "Координаты вершины (" + std::to_string(p.x) + ";" +
            std::to_string(p.y) + ") выходят за допустимый диапазон [-999, 999].";
        return false;
    }
    return true;
}

bool Validator::checkPolygonShape(const std::vector<Point>& vertices, Error& err) {
    int n = (int)vertices.size();
    // Проверка коллинеарных троек
    for (int i = 0; i < n; ++i) {
        const Point& a = vertices[i];
        const Point& b = vertices[(i + 1) % n];
        const Point& c = vertices[(i + 2) % n];
        if (checkCollinearity(a, b, c)) {
            err.type = ErrorType::invalidPolygon;
            err.errorLineNumber = i + 3; // "третья" точка в троице
            err.errorLineContent = std::to_string(c.x) + ";" + std::to_string(c.y);
            err.errorMessage = "Точки " + std::to_string(a.x) + ";" + std::to_string(a.y) +
                ", " + std::to_string(b.x) + ";" + std::to_string(b.y) +
                ", " + std::to_string(c.x) + ";" + std::to_string(c.y) + " коллинеарны.";
            return false;
        }
    }

    // Проверка пересечений рёбер (кроме соседних)
    for (int i = 0; i < n; ++i) {
        Point a1 = vertices[i];
        Point a2 = vertices[(i + 1) % n];
        for (int j = i + 2; j < n; ++j) {
            // Пропускаем соседние ребра (i,i+1) и (i+1,i+2), а также (0,n-1)
            if (j == i) continue;
            if (j == (i + 1) % n) continue;
            if (i == 0 && j == n - 1) continue;
            Point b1 = vertices[j];
            Point b2 = vertices[(j + 1) % n];
            if (checkIntersection(a1, a2, b1, b2)) {
                err.type = ErrorType::invalidPolygon;
                err.errorLineNumber = j + 2;
                err.errorLineContent = std::to_string(b2.x) + ";" + std::to_string(b2.y);
                err.errorMessage = "Входные данные формируют некорректный многоугольник: пересечение рёбер.";
                return false;
            }
        }
    }

    // Проверка невыпуклости: смотрим на векторные произведения соседних ребер
    bool gotPos = false, gotNeg = false;
    for (int i = 0; i < n; ++i) {
        const Point& p0 = vertices[i];
        const Point& p1 = vertices[(i + 1) % n];
        const Point& p2 = vertices[(i + 2) % n];
        long long cross = (long long)(p1.x - p0.x) * (p2.y - p1.y) - (long long)(p1.y - p0.y) * (p2.x - p1.x);
        if (cross > 0) gotPos = true;
        if (cross < 0) gotNeg = true;
        if (gotPos && gotNeg) break;
    }
    if (!(gotPos && gotNeg)) {
        err.type = ErrorType::invalidPolygon;
        err.errorLineNumber = 0;
        err.errorLineContent = "";
        err.errorMessage = "Многоугольник выпуклый.";
        return false;
    }

    return true;
}

bool Validator::validate(const std::vector<Point>& vertices, const Point& testPoint, Error& err) {
    // 1. Проверка количества вершин (должно быть от 3 до 1000)
    if (!checkVertexCount((int)vertices.size(), err)) return false;

    // 2. Проверка на дублирующиеся вершины
    if (!checkDuplicateVertices(vertices, err)) return false;

    // 3. Проверка диапазона координат для каждой вершины
    for (int i = 0; i < (int)vertices.size(); ++i) {
        if (!checkVertexRange(vertices[i], err, i + 2)) {  // +2 — смещение из-за строки с N и индексации с 0
            return false;
        }
    }

    // 4. Проверка порядка обхода вершин (ориентированная площадь должна быть положительной)
    if (!checkCorrectVertexOrder(vertices, err)) return false;

    // 5. Проверка простоты многоугольника и его невыпуклости
    if (!checkPolygonShape(vertices, err)) return false;

    // 6. Проверка диапазона координат тестовой точки
    if (testPoint.x < -999 || testPoint.x > 999 || testPoint.y < -999 || testPoint.y > 999) {
        err.type = ErrorType::pointOutOfRange;  // Тип ошибки — тестовая точка вне диапазона
        err.errorLineNumber = static_cast<int>(vertices.size()) + 2;  // Номер строки с точкой для проверки
        err.errorLineContent = std::to_string(testPoint.x) + ";" + std::to_string(testPoint.y);  // Координаты точки
        err.errorMessage = "Проверяемая точка (" + std::to_string(testPoint.x) + ";" + std::to_string(testPoint.y) +
            ") выходит за допустимый диапазон [-999, 999].";
        return false;
    }

    return true;  // Все проверки пройдены, данные валидны
}
