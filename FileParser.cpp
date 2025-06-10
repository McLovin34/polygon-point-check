#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <climits>

// Функция чтения данных из файла
bool FileParser::readFromFile(const std::string& fileName, std::vector<Point>& vertices, Point& testPoint, Error& err) {
    std::ifstream fin(fileName);  // Открываем файл для чтения
    err.errorInputFileWay = fileName;  // Записываем путь к файлу в объект ошибки

    // Проверяем, был ли файл открыт
    if (!fin.is_open()) {
        err.type = ErrorType::inputFileNotExist;  // Устанавливаем тип ошибки, если файл не существует
        err.errorMessage = "Неверно указан файл с входными данными. Возможно, файл не существует или нет прав на чтение.";  // Сообщение об ошибке
        return false;  // Возвращаем false, если файл не открылся
    }

    std::string line;  // Строка для хранения данных, прочитанных из файла
    int lineNumber = 0;  // Счётчик строк

    // Чтение первой строки (количество вершин)
    if (!std::getline(fin, line)) {
        err.type = ErrorType::emptyFile;  // Если файл пуст, записываем ошибку
        err.errorLineNumber = 0;  // Указываем, что ошибка на первой строке
        err.errorMessage = "Пустой файл";  // Сообщение об ошибке
        return false;  // Возвращаем false, так как файл пуст
    }
    ++lineNumber;  // Увеличиваем счётчик строк

    // Проверяем, не пуста ли строка
    if (!checkEmptyLine(line, err, lineNumber)) return false;

    // Проверяем, что первая строка не содержит лишних символов, таких как ';'
    if (line.find(';') != std::string::npos) {
        err.type = ErrorType::wrongElementCountInLine;  // Устанавливаем ошибку, если символы не соответствуют формату
        err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
        err.errorLineContent = line;  // Записываем саму строку с ошибкой
        err.errorMessage = "Некорректное количество элементов в строке: " + line +
            ". Первая строка должна содержать только количество вершин.";  // Сообщение об ошибке
        return false;  // Возвращаем false
    }

    // Преобразуем количество вершин из строки в число типа float
    float N;
    try {
        size_t pos;
        N = std::stof(line, &pos);  // Преобразуем строку в float
        if (pos != line.size()) throw std::invalid_argument("not a number");  // Если строка не была целиком преобразована в число, вызываем ошибку
    }
    catch (...) {
        err.type = ErrorType::pointNotInteger;  // Ошибка при преобразовании строки в число
        err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
        err.errorLineContent = line;  // Записываем строку с ошибкой
        err.errorMessage = "Число вершин не является числом с плавающей запятой или целым числом.";  // Сообщение об ошибке
        return false;  // Возвращаем false, так как число вершин неверно
    }

    // Проверяем, что количество вершин в допустимом диапазоне
    if (N < 3 || N > 1000) {
        err.type = ErrorType::invalidVertexCount;  // Ошибка с количеством вершин
        err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
        err.errorMessage = "Недопустимое количество вершин.";  // Сообщение об ошибке
        return false;  // Возвращаем false, если количество вершин вне диапазона
    }

    vertices.clear();  // Очищаем вектор вершин перед добавлением новых
    vertices.reserve(N);  // Резервируем память для хранения вершин

    // Чтение координат N вершин
    for (int i = 0; i < N; ++i) {
        if (!std::getline(fin, line)) {
            err.type = ErrorType::verticesMismatch;  // Ошибка, если количество вершин не совпадает с данными
            err.errorLineNumber = lineNumber + 1;  // Указываем строку с ошибкой
            err.errorMessage = "Не хватает координат для всех вершин.";  // Сообщение об ошибке
            return false;  // Возвращаем false
        }
        ++lineNumber;  // Увеличиваем номер строки

        float x, y;
        if (!parsePointLine(line, x, y)) {  // Парсим координаты из строки
            err.type = ErrorType::pointNotInteger;  // Ошибка при преобразовании координат
            err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
            err.errorMessage = "Некорректный формат координат для вершины.";  // Сообщение об ошибке
            return false;  // Возвращаем false
        }

        // Проверяем, что координаты вершин находятся в допустимом диапазоне
        if (!checkOutOfRangeCoordinates(x, y, err, lineNumber, true)) return false;

        vertices.emplace_back(x, y);  // Добавляем вершину в вектор
    }

    // Чтение координат тестовой точки
    if (!std::getline(fin, line)) {
        err.type = ErrorType::verticesMismatch;  // Ошибка при чтении тестовой точки
        err.errorLineNumber = lineNumber + 1;  // Указываем строку с ошибкой
        err.errorMessage = "Не хватает данных для тестовой точки.";  // Сообщение об ошибке
        return false;  // Возвращаем false
    }

    ++lineNumber;  // Увеличиваем номер строки
    float tx, ty;
    if (!parsePointLine(line, tx, ty)) {  // Парсим координаты тестовой точки
        err.type = ErrorType::pointNotInteger;  // Ошибка при преобразовании координат
        err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
        err.errorMessage = "Некорректные координаты тестовой точки.";  // Сообщение об ошибке
        return false;  // Возвращаем false
    }

    // Проверяем, что координаты тестовой точки находятся в допустимом диапазоне
    if (!checkOutOfRangeCoordinates(tx, ty, err, lineNumber, false)) return false;

    testPoint = Point(tx, ty);  // Сохраняем тестовую точку

    return true;  // Возвращаем true, если все данные успешно считаны
}

// Проверка, что строка не пуста
bool FileParser::checkEmptyLine(const std::string& line, Error& err, int lineNumber) {
    if (line.empty()) {  // Если строка пуста
        err.type = ErrorType::emptyLineFound;  // Ошибка с пустой строкой
        err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
        err.errorLineContent = "";  // Пустая строка
        err.errorMessage = "Обнаружена пустая строка во входных данных. Удалите лишние строки.";  // Сообщение об ошибке
        return false;  // Возвращаем false
    }
    return true;  // Строка не пуста
}

// Проверка, что в строке ровно один символ ';'
bool FileParser::checkInvalidElementCount(const std::string& line, Error& err, int lineNumber) {
    size_t countSemicolons = 0;  // Счётчик точек с запятой
    for (char c : line) {
        if (c == ';') ++countSemicolons;  // Увеличиваем счётчик, если символ ';'
    }

    if (countSemicolons != 1) {  // Если точка с запятой встречается не один раз
        err.type = ErrorType::wrongElementCountInLine;  // Ошибка в количестве элементов
        err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
        err.errorLineContent = line;  // Записываем строку с ошибкой
        err.errorMessage = "Некорректное количество элементов в строке: " + line +
            ". Каждая точка должна содержать два целых числа, разделённых ;.";  // Сообщение об ошибке
        return false;  // Возвращаем false
    }
    return true;  // Строка корректна
}

// Проверка, что все символы в строке допустимы
bool FileParser::checkInvalidCharacters(const std::string& line, Error& err, int lineNumber) {
    for (char c : line) {
        if (!(std::isdigit(static_cast<unsigned char>(c)) || c == '-' || c == ';')) {  // Проверка на цифры, '-' или ';'
            err.type = ErrorType::invalidCharacters;  // Ошибка с недопустимыми символами
            err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
            err.errorLineContent = line;  // Записываем строку с ошибкой
            err.errorMessage = "Входные данные содержат некорректные символы. Разрешены только числа, точки с запятой и переводы строк.";  // Сообщение об ошибке
            return false;  // Возвращаем false
        }
    }
    return true;  // Все символы корректны
}

// Проверка, что координаты можно преобразовать в числа с плавающей запятой или целые
bool FileParser::checkNonIntegerCoordinates(const std::string& line, Error& err, int lineNumber) {
    float x, y;
    if (!parsePointLine(line, x, y)) {  // Парсим координаты
        err.type = ErrorType::pointNotInteger;  // Ошибка при парсинге
        err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
        err.errorLineContent = line;  // Записываем строку с ошибкой
        err.errorMessage = "Координата не является числом с плавающей запятой или целым числом.";  // Сообщение об ошибке
        return false;  // Возвращаем false
    }
    return true;  // Координаты корректны
}

// Функция парсинга строки в точку
bool FileParser::parsePointLine(const std::string& line, float& x, float& y) {
    size_t sep = line.find(';');  // Находим разделитель ';'
    if (sep == std::string::npos) return false;  // Если не нашли разделитель, возвращаем false

    std::string xs = line.substr(0, sep);  // Читаем первую часть (x)
    std::string ys = line.substr(sep + 1);  // Читаем вторую часть (y)

    if (xs.empty() || ys.empty()) return false;  // Если хотя бы одна часть пустая, возвращаем false

    try {
        size_t pos;
        float lx = std::stof(xs, &pos);  // Преобразуем первую часть в float
        if (pos != xs.size()) return false;  // Если не вся строка была преобразована в число, возвращаем false
        float ly = std::stof(ys, &pos);  // Преобразуем вторую часть в float
        if (pos != ys.size()) return false;  // Если не вся строка была преобразована в число, возвращаем false
        if (lx < -999.0f || lx > 999.0f || ly < -999.0f || ly > 999.0f) return false;  // Проверяем диапазон
        x = lx;  // Присваиваем x
        y = ly;  // Присваиваем y
        return true;  // Возвращаем true, если всё прошло успешно
    }
    catch (...) {
        return false;  // Если произошла ошибка, возвращаем false
    }
}

// Проверка, что координаты находятся в допустимом диапазоне
bool FileParser::checkOutOfRangeCoordinates(float x, float y, Error& err, int lineNumber, bool isVertex) {
    if (x < -999.0f || x > 999.0f || y < -999.0f || y > 999.0f) {  // Проверка диапазона
        if (isVertex) {
            err.type = ErrorType::coordinateOutOfRange;  // Ошибка с координатами вершины
            err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
            err.errorLineContent = std::to_string(x) + ";" + std::to_string(y);  // Записываем координаты
            err.errorMessage = "Координаты вершины (" + std::to_string(x) + ";" + std::to_string(y) +
                ") выходят за допустимый диапазон [-999, 999].";  // Сообщение об ошибке
        }
        else {
            err.type = ErrorType::pointOutOfRange;  // Ошибка с координатами точки
            err.errorLineNumber = lineNumber;  // Указываем строку с ошибкой
            err.errorLineContent = std::to_string(x) + ";" + std::to_string(y);  // Записываем координаты
            err.errorMessage = "Проверяемая точка (" + std::to_string(x) + ";" + std::to_string(y) +
                ") выходит за допустимый диапазон [-999, 999].";  // Сообщение об ошибке
        }
        return false;  // Возвращаем false
    }
    return true;  // Возвращаем true, если всё в порядке
}
