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

    // Записываем путь к входному файлу в объект ошибки (для сообщения об ошибках)
    err.errorInputFileWay = fileName;

    // Проверяем, успешно ли открылся файл
    if (!fin.is_open()) {
        // Если файл не открыт, устанавливаем тип ошибки "входной файл не существует"
        err.type = ErrorType::inputFileNotExist;
        // Записываем дополнительное сообщение об ошибке
        err.errorMessage = "Неверно указан файл с входными данными. Возможно, файл не существует или нет прав на чтение.";
        // Возвращаем false — файл не удалось открыть
        return false;
    }

    std::string line;
    int lineNumber = 0;

    // Чтение первой строки: количество вершин
    if (!std::getline(fin, line)) {
        // Пустой файл
        err.type = ErrorType::emptyFile;
        err.errorLineNumber = 0;
        err.errorLineContent = "";
        err.errorMessage = "Пустой файл";
        return false;
    }
    ++lineNumber;

    if (!checkEmptyLine(line, err, lineNumber)) return false;

    // Первая строка НЕ должна содержать символ ';'
    if (line.find(';') != std::string::npos) {
        err.type = ErrorType::wrongElementCountInLine;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Некорректное количество элементов в строке: " + line +
            ". Первая строка должна содержать только количество вершин.";
        return false;
    }

    // Попробуем преобразовать в int
    int N;
    try {
        size_t pos;
        long tmp = std::stol(line, &pos);
        if (pos != line.size()) throw std::invalid_argument("not integer");
        N = static_cast<int>(tmp);
    }
    catch (...) {
        err.type = ErrorType::pointNotInteger;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Координата не является целым числом. Допустимы только целые числа.";
        return false;
    }

    // Проверка диапазона N: [3, 1000]
    if (N < 3 || N > 1000) {
        err.type = ErrorType::invalidVertexCount;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Некорректное количество вершин. Допустимый диапазон: [3, 1000].";
        return false;
    }

    vertices.clear();
    vertices.reserve(N);

    // Чтение N строк с вершинами
    for (int i = 0; i < N; ++i) {
        if (!std::getline(fin, line)) {
            // Недостаточно строк с вершинами
            err.type = ErrorType::verticesMismatch;
            err.errorLineNumber = lineNumber + 1;
            err.errorLineContent = "";
            err.errorMessage = "Обнаружено несоответствие: число вершин не совпадает с количеством заданных координат. Добавьте или уберите координаты.";
            return false;
        }
        ++lineNumber;

        if (!checkEmptyLine(line, err, lineNumber)) return false;
        if (!checkInvalidElementCount(line, err, lineNumber)) return false;
        if (!checkInvalidCharacters(line, err, lineNumber)) return false;

        int x, y;
        if (!parsePointLine(line, x, y)) {
            err.type = ErrorType::pointNotInteger;
            err.errorLineNumber = lineNumber;
            err.errorLineContent = line;
            err.errorMessage = "Координата не является целым числом. Допустимы только целые числа.";
            return false;
        }

        if (!checkOutOfRangeCoordinates(x, y, err, lineNumber, true)) return false;

        vertices.emplace_back(x, y);
    }

    // Чтение последней строки: проверяемая точка
    if (!std::getline(fin, line)) {
        err.type = ErrorType::verticesMismatch;
        err.errorLineNumber = lineNumber + 1;
        err.errorLineContent = "";
        err.errorMessage = "Обнаружено несоответствие: число вершин не совпадает с количеством заданных координат. Добавьте или уберите координаты.";
        return false;
    }
    ++lineNumber;

    if (!checkEmptyLine(line, err, lineNumber)) return false;
    if (!checkInvalidElementCount(line, err, lineNumber)) return false;
    if (!checkInvalidCharacters(line, err, lineNumber)) return false;

    int tx, ty;
    if (!parsePointLine(line, tx, ty)) {
        err.type = ErrorType::pointNotInteger;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "Координата не является целым числом. Допустимы только целые числа.";
        return false;
    }
    if (!checkOutOfRangeCoordinates(tx, ty, err, lineNumber, false)) return false;
    testPoint = Point(tx, ty);

    // Проверка на наличие лишних строк (непустых)
    if (std::getline(fin, line)) {
        bool onlySpaces = true;
        for (char c : line) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                onlySpaces = false;
                break;
            }
        }
        if (!onlySpaces) {
            err.type = ErrorType::verticesMismatch;
            err.errorLineNumber = lineNumber + 1;
            err.errorLineContent = line;
            err.errorMessage = "Обнаружено несоответствие: дополнительные строки после координат.";
            return false;
        }
    }

    return true;
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
