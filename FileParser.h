#pragma once

#include "Error.h"
#include "Point.h"
#include <string>
#include <vector>

/// Класс, отвечающий за чтение и базовый синтаксический анализ входного файла.
/// После вызова readFromFile() можно получить вектор вершин и проверяемую точку.
class FileParser {
public:
    FileParser() = default;

    /// \brief Считывает данные о многоугольнике и проверяемой точке из входного файла,
    ///        проверяет базовый формат (разделитель ';', целые, диапазон).
    /// \param[in]   fileName  – путь к входному файлу.
    /// \param[out]  vertices  – вектор вершин многоугольника (если успешно).
    /// \param[out]  testPoint – проверяемая точка (если успешно).
    /// \param[out]  err       – объект Error, куда записываются сведения об ошибках.
    /// \return true, если файл успешно прочитан и синтаксически корректен; false — при первой найденной ошибке.
    bool readFromFile(const std::string& fileName,
        std::vector<Point>& vertices,
        Point& testPoint,
        Error& err);

private:
    // --- Вспомогательные private-методы для поэтапного синтаксического анализа ---

    /// Проверка, что строка не пуста
    bool checkEmptyLine(const std::string& line, Error& err, int lineNumber);

    /// Проверка, что в строке ровно одна точка с запятой (';')
    bool checkInvalidElementCount(const std::string& line, Error& err, int lineNumber);

    /// Проверка, что все символы допустимы (цифры, '-', ';')
    bool checkInvalidCharacters(const std::string& line, Error& err, int lineNumber);

    /// Проверка, что строка может быть распознана как две целые части "x;y"
    bool checkNonIntegerCoordinates(const std::string& line, Error& err, int lineNumber);

    /// Парсинг строки "x;y" → два int (возвращает false, если не удалось)
    bool parsePointLine(const std::string& line, int& x, int& y);

    /// Проверка, что координаты (x, y) лежат в диапазоне [-999, 999]
    /// isVertex = true → проверяем вершину, иначе — проверяемая точка
    bool checkOutOfRangeCoordinates(int x, int y, Error& err, int lineNumber, bool isVertex);
};

