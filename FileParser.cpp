#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <climits>

bool FileParser::readFromFile(const std::string& fileName,
    std::vector<Point>& vertices,
    Point& testPoint,
    Error& err) {
    // Открываем входной файл для чтения по имени fileName
    std::ifstream fin(fileName);

    // Записываем путь к входному файлу в объект ошибки (для информативности)
    err.errorInputFileWay = fileName;

    // Проверяем, успешно ли открыт файл
    if (!fin.is_open()) {
        // Если файл не открыт — устанавливаем тип ошибки "входной файл не существует"
        err.type = ErrorType::inputFileNotExist;
        // Записываем детальное сообщение об ошибке
        err.errorMessage = "Неверно указан файл с входными данными. Возможно, файл не существует или нет прав на чтение.";
        // Возвращаем false — невозможно продолжить
        return false;
    }

    std::string line;   // Строка для хранения текущей строки файла
    int lineNumber = 0; // Счётчик номера текущей строки (для отчётности об ошибках)

    // Чтение первой строки: количество вершин
    if (!std::getline(fin, line)) {
        // Если не удалось прочитать первую строку — файл пуст
        err.type = ErrorType::emptyFile;
        err.errorLineNumber = 0;
        err.errorLineContent = "";
        err.errorMessage = "Пустой файл";
        return false;
    }
    ++lineNumber; // Увеличиваем номер строки (теперь lineNumber = 1)

    // Проверка: не является ли строка пустой или состоящей только из пробелов
    if (!checkEmptyLine(line, err, lineNumber)) return false;

    // Первая строка НЕ должна содержать символ ';'
    if (line.find(';') != std::string::npos) {
        // Если есть лишние элементы — ошибка формата первой строки
        err.type = ErrorType::wrongElementCountInLine;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Некорректное количество элементов в строке: " + line +
            ". Первая строка должна содержать только количество вершин.";
        return false;
    }

    // Попытка преобразовать строку в целое число N (количество вершин)
    int N;
    try {
        size_t pos;
        long tmp = std::stol(line, &pos);         // Преобразуем строку в long
        if (pos != line.size()) throw std::invalid_argument("not integer"); // Проверка, что вся строка — число
        N = static_cast<int>(tmp);
    }
    catch (...) {
        // Если не получилось преобразовать — ошибка: не целое число
        err.type = ErrorType::pointNotInteger;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Координата не является целым числом. Допустимы только целые числа.";
        return false;
    }

    // Проверяем, что N в диапазоне [3, 1000]
    if (N < 3 || N > 1000) {
        err.type = ErrorType::invalidVertexCount;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Некорректное количество вершин. Допустимый диапазон: [3, 1000].";
        return false;
    }

    vertices.clear();      // Очищаем массив вершин
    vertices.reserve(N);   // Резервируем память под N вершин

    // Чтение N строк с вершинами полигона
    for (int i = 0; i < N; ++i) {
        if (!std::getline(fin, line)) {
            // Если строк с вершинами меньше, чем нужно — ошибка: не хватает координат
            err.type = ErrorType::verticesMismatch;
            err.errorLineNumber = lineNumber + 1;
            err.errorLineContent = "";
            err.errorMessage = "Обнаружено несоответствие: число вершин не совпадает с количеством заданных координат. Добавьте или уберите координаты.";
            return false;
        }
        ++lineNumber; // Переходим к следующей строке

        // Проверка: строка не пуста
        if (!checkEmptyLine(line, err, lineNumber)) return false;
        // Проверка: количество элементов в строке (например, не больше двух координат)
        if (!checkInvalidElementCount(line, err, lineNumber)) return false;
        // Проверка: нет ли некорректных символов
        if (!checkInvalidCharacters(line, err, lineNumber)) return false;

        int x, y;
        // Парсим координаты из строки (должны быть два целых числа)
        if (!parsePointLine(line, x, y)) {
            // Если не удалось распарсить — ошибка: не целое число
            err.type = ErrorType::pointNotInteger;
            err.errorLineNumber = lineNumber;
            err.errorLineContent = line;
            err.errorMessage = "Координата не является целым числом. Допустимы только целые числа.";
            return false;
        }

        // Проверяем, что координаты не выходят за допустимый диапазон
        if (!checkOutOfRangeCoordinates(x, y, err, lineNumber, true)) return false;

        // Добавляем вершину в массив
        vertices.emplace_back(x, y);
    }

    // Чтение последней строки: координаты проверяемой точки
    if (!std::getline(fin, line)) {
        // Если нет строки для точки — ошибка: не хватает данных
        err.type = ErrorType::verticesMismatch;
        err.errorLineNumber = lineNumber + 1;
        err.errorLineContent = "";
        err.errorMessage = "Обнаружено несоответствие: число вершин не совпадает с количеством заданных координат. Добавьте или уберите координаты.";
        return false;
    }
    ++lineNumber; // Счётчик строк

    // Проверяем, что строка не пустая
    if (!checkEmptyLine(line, err, lineNumber)) return false;
    // Проверяем, что количество элементов в строке корректное (только две координаты)
    if (!checkInvalidElementCount(line, err, lineNumber)) return false;
    // Проверяем отсутствие некорректных символов
    if (!checkInvalidCharacters(line, err, lineNumber)) return false;

    int tx, ty;
    // Парсим координаты проверяемой точки
    if (!parsePointLine(line, tx, ty)) {
        err.type = ErrorType::pointNotInteger;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Координата не является целым числом. Допустимы только целые числа.";
        return false;
    }
    // Проверяем диапазон координат точки
    if (!checkOutOfRangeCoordinates(tx, ty, err, lineNumber, false)) return false;
    // Запоминаем проверяемую точку
    testPoint = Point(tx, ty);

    // Проверка на наличие лишних строк (например, если после последней строки есть ещё непустые строки)
    if (std::getline(fin, line)) {
        bool onlySpaces = true; // Флаг: строка содержит только пробелы/табуляции
        for (char c : line) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                onlySpaces = false;
                break;
            }
        }
        if (!onlySpaces) {
            // Если есть ещё содержательные строки — ошибка
            err.type = ErrorType::verticesMismatch;
            err.errorLineNumber = lineNumber + 1;
            err.errorLineContent = line;
            err.errorMessage = "Обнаружено несоответствие: дополнительные строки после координат.";
            return false;
        }
    }

    return true; // Всё успешно
}

bool FileParser::checkEmptyLine(const std::string& line, Error& err, int lineNumber) {
    if (line.empty()) {
        err.type = ErrorType::emptyLineFound;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = "";
        err.errorMessage = "Обнаружена пустая строка во входных данных. Удалите лишние строки.";
        return false;
    }
    return true;
}

bool FileParser::checkInvalidElementCount(const std::string& line, Error& err, int lineNumber) {
    size_t countSemicolons = 0;
    for (char c : line) {
        if (c == ';') ++countSemicolons;
    }
    if (countSemicolons != 1) {
        err.type = ErrorType::wrongElementCountInLine;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Некорректное количество элементов в строке: " + line +
            ". Каждая точка должна содержать два целых числа, разделённых ;.";
        return false;
    }
    return true;
}

bool FileParser::checkInvalidCharacters(const std::string& line, Error& err, int lineNumber) {
    for (char c : line) {
        if (!(std::isdigit(static_cast<unsigned char>(c)) || c == '-' || c == ';')) {
            err.type = ErrorType::invalidCharacters;
            err.errorLineNumber = lineNumber;
            err.errorLineContent = line;
            err.errorMessage = "Входные данные содержат некорректные символы. Разрешены только числа, точки с запятой и переводы строк.";
            return false;
        }
    }
    return true;
}

bool FileParser::checkNonIntegerCoordinates(const std::string& line, Error& err, int lineNumber) {
    int x, y;
    if (!parsePointLine(line, x, y)) {
        err.type = ErrorType::pointNotInteger;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Координата не является целым числом. Допустимы только целые числа.";
        return false;
    }
    return true;
}

bool FileParser::parsePointLine(const std::string& line, int& x, int& y) {
    size_t sep = line.find(';');
    if (sep == std::string::npos) return false;
    std::string xs = line.substr(0, sep);
    std::string ys = line.substr(sep + 1);

    if (xs.empty() || ys.empty()) return false;

    try {
        size_t pos;
        long lx = std::stol(xs, &pos);
        if (pos != xs.size()) return false;
        long ly = std::stol(ys, &pos);
        if (pos != ys.size()) return false;
        if (lx < INT_MIN || lx > INT_MAX || ly < INT_MIN || ly > INT_MAX) return false;
        x = static_cast<int>(lx);
        y = static_cast<int>(ly);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool FileParser::checkOutOfRangeCoordinates(int x, int y, Error& err, int lineNumber, bool isVertex) {
    if (x < -999 || x > 999 || y < -999 || y > 999) {
        if (isVertex) {
            err.type = ErrorType::coordinateOutOfRange;
            err.errorLineNumber = lineNumber;
            err.errorLineContent = std::to_string(x) + ";" + std::to_string(y);
            err.errorMessage = "Координаты вершины (" + std::to_string(x) + ";" + std::to_string(y) +
                ") выходят за допустимый диапазон [-999, 999].";
        }
        else {
            err.type = ErrorType::pointOutOfRange;
            err.errorLineNumber = lineNumber;
            err.errorLineContent = std::to_string(x) + ";" + std::to_string(y);
            err.errorMessage = "Проверяемая точка (" + std::to_string(x) + ";" + std::to_string(y) +
                ") выходит за допустимый диапазон [-999, 999].";
        }
        return false;
    }
    return true;
}
