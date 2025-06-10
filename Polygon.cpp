#include "Polygon.h"
#include <climits>
#include <set> 

// private: вычисление ориентированной площади (удвоенной) — формула Гаусса
long long Polygon::signedArea() const {
    long long area2 = 0;  // Переменная для хранения удвоенной площади
    int n = (int)vertices.size();  // Количество вершин в многоугольнике
    for (int i = 0; i < n; ++i) {  // Проходим по всем вершинам
        int j = (i + 1) % n;  // Индекс следующей вершины (с учётом замыкания контура)
        // Прибавляем разность произведений координат (формула Гаусса)
        area2 += (long long)vertices[i].x * vertices[j].y - (long long)vertices[j].x * vertices[i].y;
    }
    return area2;  // Возвращаем удвоенную площадь (может быть отрицательной — зависит от ориентации)
}

// private: определение ориентации трёх точек (p, q, r)
int Polygon::orientation(const Point& p, const Point& q, const Point& r) const {
    // Вычисляем ориентированное значение (площадь параллелограмма)
    long long val = (long long)(q.y - p.y) * (r.x - q.x) - (long long)(q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;            // 0 — точки коллинеарны
    return (val > 0) ? 1 : 2;          // 1 — по часовой, 2 — против часовой стрелки
}

// private: проверка, что точка q лежит на отрезке pr
bool Polygon::onSegment(const Point& p, const Point& q, const Point& r) const {
    // Проверяем, что координаты q лежат между p и r по обеим осям
    return (std::min(p.x, r.x) <= q.x && q.x <= std::max(p.x, r.x) &&
        std::min(p.y, r.y) <= q.y && q.y <= std::max(p.y, r.y));
}

// Проверка, что три точки лежат на одной прямой (коллинеарность)
bool Polygon::checkCollinearity(const Point& a, const Point& b, const Point& c) const {
    // Вычисляем векторное произведение для проверки коллинеарности
    long long cross = (long long)(b.x - a.x) * (c.y - a.y) - (long long)(b.y - a.y) * (c.x - a.x);
    return (cross == 0);  // Если ноль — точки коллинеарны
}

// Проверка на пересечение двух отрезков: [a1,a2] и [b1,b2]
bool Polygon::checkIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) const {
    int o1 = orientation(a1, a2, b1);  // Ориентация тройки (a1, a2, b1)
    int o2 = orientation(a1, a2, b2);  // Ориентация тройки (a1, a2, b2)
    int o3 = orientation(b1, b2, a1);  // Ориентация тройки (b1, b2, a1)
    int o4 = orientation(b1, b2, a2);  // Ориентация тройки (b1, b2, a2)

    // Общий случай: если ориентации разные, отрезки пересекаются
    if (o1 != o2 && o3 != o4) return true;

    // Частные случаи: коллинеарность и попадание одной из точек на другой отрезок
    if (o1 == 0 && onSegment(a1, b1, a2)) return true;  // b1 лежит на a1a2
    if (o2 == 0 && onSegment(a1, b2, a2)) return true;  // b2 лежит на a1a2
    if (o3 == 0 && onSegment(b1, a1, b2)) return true;  // a1 лежит на b1b2
    if (o4 == 0 && onSegment(b1, a2, b2)) return true;  // a2 лежит на b1b2

    return false;  // Отрезки не пересекаются
}

bool Polygon::checkPolygonShape(Error& err) const {
    int n = (int)vertices.size();  // Получаем количество вершин многоугольника

    // Проверка: среди каждой последовательной тройки вершин не должно быть коллинеарных
    for (int i = 0; i < n; ++i) {
        const Point& a = vertices[i];
        const Point& b = vertices[(i + 1) % n];
        const Point& c = vertices[(i + 2) % n];
        if (checkCollinearity(a, b, c)) {  // Если три последовательные вершины лежат на одной прямой
            err.type = ErrorType::invalidPolygon;  // Устанавливаем тип ошибки: некорректный многоугольник
            err.errorLineNumber = i + 3;  // Указываем номер строки с ошибочной вершиной
            err.errorLineContent = std::to_string(c.x) + ";" + std::to_string(c.y);  // Сохраняем координаты этой вершины
            err.errorMessage = "Точки " + std::to_string(a.x) + ";" + std::to_string(a.y) +
                ", " + std::to_string(b.x) + ";" + std::to_string(b.y) +
                ", " + std::to_string(c.x) + ";" + std::to_string(c.y) + " коллинеарны.";  // Подробное сообщение
            return false;  // Возвращаем false — найден некорректный участок
        }
    }

    // Проверка: отсутствие самопересечений (никакие не-соседние рёбра не должны пересекаться)
    for (int i = 0; i < n; ++i) {
        Point a1 = vertices[i];
        Point a2 = vertices[(i + 1) % n];
        for (int j = i + 2; j < n; ++j) {
            if (j == i) continue;  // Пропускаем совпадающие индексы (невозможно)
            if (j == (i + 1) % n) continue;  // Пропускаем смежные рёбра (имеют общую вершину)
            if (i == 0 && j == n - 1) continue;  // Пропускаем первое и последнее рёбра (смежные по кругу)
            Point b1 = vertices[j];
            Point b2 = vertices[(j + 1) % n];
            if (checkIntersection(a1, a2, b1, b2)) {  // Проверяем пересечение двух рёбер
                err.type = ErrorType::invalidPolygon;  // Ошибка: некорректный многоугольник
                err.errorLineNumber = j + 2;  // Указываем номер строки второй вершины второго ребра
                err.errorLineContent = std::to_string(b2.x) + ";" + std::to_string(b2.y);  // Сохраняем координаты вершины
                err.errorMessage = "Входные данные формируют некорректный многоугольник: пересечение рёбер.";  // Подробное сообщение
                return false;  // Возвращаем false — найдено пересечение рёбер
            }
        }
    }

    // Проверка невыпуклости: ищем наличие разных знаков у векторных произведений соседних рёбер
    bool gotPos = false, gotNeg = false;
    for (int i = 0; i < n; ++i) {
        const Point& p0 = vertices[i];
        const Point& p1 = vertices[(i + 1) % n];
        const Point& p2 = vertices[(i + 2) % n];
        // Векторное произведение (для определения направления "поворота")
        long long cross = (long long)(p1.x - p0.x) * (p2.y - p1.y) - (long long)(p1.y - p0.y) * (p2.x - p1.x);
        if (cross > 0) gotPos = true;
        if (cross < 0) gotNeg = true;
        if (gotPos && gotNeg) break;  // Если нашли и положительный и отрицательный знак — многоугольник невыпуклый
    }
    if (!(gotPos && gotNeg)) {  // Если векторные произведения только одного знака — многоугольник выпуклый
        err.type = ErrorType::invalidPolygon;  // Ошибка: многоугольник не невыпуклый (выпуклый)
        err.errorLineNumber = 0;
        err.errorLineContent = "";
        err.errorMessage = "Многоугольник выпуклый.";  // Подробное сообщение
        return false;  // Возвращаем false — многоугольник не проходит проверку на невыпуклость
    }

    return true;  // Все проверки пройдены успешно: многоугольник невыпуклый, без коллинеарных троек и самопересечений
}

bool Polygon::isValid(Error& err) const {
    int n = (int)vertices.size();
    if (n < 3) {
        err.type = ErrorType::invalidVertexCount;
        err.errorLineNumber = 1;
        err.errorLineContent = std::to_string(n);
        err.errorMessage = "Недостаточно вершин: " + std::to_string(n);
        return false;
    }
    if (n > 1000) {
        err.type = ErrorType::invalidVertexCount;
        err.errorLineNumber = 1;
        err.errorLineContent = std::to_string(n);
        err.errorMessage = "Количество вершин = " + std::to_string(n) + " (максимум 1000)";
        return false;
    }
    // Проверка диапазона каждой вершины
    for (int i = 0; i < n; ++i) {
        if (vertices[i].x < -999 || vertices[i].x > 999 || vertices[i].y < -999 || vertices[i].y > 999) {
            err.type = ErrorType::coordinateOutOfRange;
            err.errorLineNumber = i + 2;
            err.errorLineContent = std::to_string(vertices[i].x) + ";" + std::to_string(vertices[i].y);
            err.errorMessage = "Координаты вершины (" + std::to_string(vertices[i].x) + ";" +
                std::to_string(vertices[i].y) + ") выходят за допустимый диапазон [-999, 999].";
            return false;
        }
    }
    // Дубликаты
    {
        std::set<Point> seen;
        for (int i = 0; i < n; ++i) {
            if (seen.find(vertices[i]) != seen.end()) {
                err.type = ErrorType::duplicateVertex;
                err.errorLineNumber = i + 2;
                err.errorLineContent = std::to_string(vertices[i].x) + ";" + std::to_string(vertices[i].y);
                err.errorMessage = "Входные данные содержат дублирующиеся вершины: (" +
                    std::to_string(vertices[i].x) + ";" + std::to_string(vertices[i].y) + ").";
                return false;
            }
            seen.insert(vertices[i]);
        }
    }
    // Порядок обхода
    long long area2 = signedArea();
    if (area2 <= 0) {
        err.type = ErrorType::wrongOrder;
        err.errorLineNumber = 0;
        err.errorLineContent = "";
        err.errorMessage = "Вершины многоугольника заданы не в порядке обхода. Упорядочьте их корректно.";
        return false;
    }
    // Форма (коллинеарность, пересечения, невыпуклость)
    if (!checkPolygonShape(err)) {
        return false;
    }
    return true;
}

bool Polygon::contains(const Point& p) const {
    int n = (int)vertices.size();
    bool inside = false;
    for (int i = 0; i < n; ++i) {
        const Point& a = vertices[i];
        const Point& b = vertices[(i + 1) % n];
        // Если точка лежит на ребре
        if (orientation(a, b, p) == 0 && onSegment(a, p, b)) {
            return true;
        }
        bool cond1 = (a.y > p.y) != (b.y > p.y);
        if (cond1) {
            double xinters = (double)(b.x - a.x) * (double)(p.y - a.y) / (double)(b.y - a.y) + a.x;
            if (xinters == p.x) {
                return true;
            }
            if (xinters > p.x) {
                inside = !inside;
            }
        }
    }
    return inside;
}
