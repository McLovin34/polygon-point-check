#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <climits>

// ������� ������ ������ �� �����
bool FileParser::readFromFile(const std::string& fileName, std::vector<Point>& vertices, Point& testPoint, Error& err) {
    std::ifstream fin(fileName);  // ��������� ���� ��� ������
    err.errorInputFileWay = fileName;  // ���������� ���� � ����� � ������ ������

    // ���������, ��� �� ���� ������
    if (!fin.is_open()) {
        err.type = ErrorType::inputFileNotExist;  // ������������� ��� ������, ���� ���� �� ����������
        err.errorMessage = "������� ������ ���� � �������� �������. ��������, ���� �� ���������� ��� ��� ���� �� ������.";  // ��������� �� ������
        return false;  // ���������� false, ���� ���� �� ��������
    }

    std::string line;  // ������ ��� �������� ������, ����������� �� �����
    int lineNumber = 0;  // ������� �����

    // ������ ������ ������ (���������� ������)
    if (!std::getline(fin, line)) {
        err.type = ErrorType::emptyFile;  // ���� ���� ����, ���������� ������
        err.errorLineNumber = 0;  // ���������, ��� ������ �� ������ ������
        err.errorMessage = "������ ����";  // ��������� �� ������
        return false;  // ���������� false, ��� ��� ���� ����
    }
    ++lineNumber;  // ����������� ������� �����

    // ���������, �� ����� �� ������
    if (!checkEmptyLine(line, err, lineNumber)) return false;

    // ���������, ��� ������ ������ �� �������� ������ ��������, ����� ��� ';'
    if (line.find(';') != std::string::npos) {
        err.type = ErrorType::wrongElementCountInLine;  // ������������� ������, ���� ������� �� ������������� �������
        err.errorLineNumber = lineNumber;  // ��������� ������ � �������
        err.errorLineContent = line;  // ���������� ���� ������ � �������
        err.errorMessage = "������������ ���������� ��������� � ������: " + line +
            ". ������ ������ ������ ��������� ������ ���������� ������.";  // ��������� �� ������
        return false;  // ���������� false
    }

    // ����������� ���������� ������ �� ������ � ����� ���� float
    float N;
    try {
        size_t pos;
        N = std::stof(line, &pos);  // ����������� ������ � float
        if (pos != line.size()) throw std::invalid_argument("not a number");  // ���� ������ �� ���� ������� ������������� � �����, �������� ������
    }
    catch (...) {
        err.type = ErrorType::pointNotInteger;  // ������ ��� �������������� ������ � �����
        err.errorLineNumber = lineNumber;  // ��������� ������ � �������
        err.errorLineContent = line;  // ���������� ������ � �������
        err.errorMessage = "����� ������ �� �������� ������ � ��������� ������� ��� ����� ������.";  // ��������� �� ������
        return false;  // ���������� false, ��� ��� ����� ������ �������
    }

    // ���������, ��� ���������� ������ � ���������� ���������
    if (N < 3 || N > 1000) {
        err.type = ErrorType::invalidVertexCount;  // ������ � ����������� ������
        err.errorLineNumber = lineNumber;  // ��������� ������ � �������
        err.errorMessage = "������������ ���������� ������.";  // ��������� �� ������
        return false;  // ���������� false, ���� ���������� ������ ��� ���������
    }

    vertices.clear();  // ������� ������ ������ ����� ����������� �����
    vertices.reserve(N);  // ����������� ������ ��� �������� ������

    // ������ ��������� N ������
    for (int i = 0; i < N; ++i) {
        if (!std::getline(fin, line)) {
            err.type = ErrorType::verticesMismatch;  // ������, ���� ���������� ������ �� ��������� � �������
            err.errorLineNumber = lineNumber + 1;  // ��������� ������ � �������
            err.errorMessage = "�� ������� ��������� ��� ���� ������.";  // ��������� �� ������
            return false;  // ���������� false
        }
        ++lineNumber;  // ����������� ����� ������

        float x, y;
        if (!parsePointLine(line, x, y)) {  // ������ ���������� �� ������
            err.type = ErrorType::pointNotInteger;  // ������ ��� �������������� ���������
            err.errorLineNumber = lineNumber;  // ��������� ������ � �������
            err.errorMessage = "������������ ������ ��������� ��� �������.";  // ��������� �� ������
            return false;  // ���������� false
        }

        // ���������, ��� ���������� ������ ��������� � ���������� ���������
        if (!checkOutOfRangeCoordinates(x, y, err, lineNumber, true)) return false;

        vertices.emplace_back(x, y);  // ��������� ������� � ������
    }

    // ������ ��������� �������� �����
    if (!std::getline(fin, line)) {
        err.type = ErrorType::verticesMismatch;  // ������ ��� ������ �������� �����
        err.errorLineNumber = lineNumber + 1;  // ��������� ������ � �������
        err.errorMessage = "�� ������� ������ ��� �������� �����.";  // ��������� �� ������
        return false;  // ���������� false
    }

    ++lineNumber;  // ����������� ����� ������
    float tx, ty;
    if (!parsePointLine(line, tx, ty)) {  // ������ ���������� �������� �����
        err.type = ErrorType::pointNotInteger;  // ������ ��� �������������� ���������
        err.errorLineNumber = lineNumber;  // ��������� ������ � �������
        err.errorMessage = "������������ ���������� �������� �����.";  // ��������� �� ������
        return false;  // ���������� false
    }

    // ���������, ��� ���������� �������� ����� ��������� � ���������� ���������
    if (!checkOutOfRangeCoordinates(tx, ty, err, lineNumber, false)) return false;

    testPoint = Point(tx, ty);  // ��������� �������� �����

    return true;  // ���������� true, ���� ��� ������ ������� �������
}

// ��������, ��� ������ �� �����
bool FileParser::checkEmptyLine(const std::string& line, Error& err, int lineNumber) {
    if (line.empty()) {  // ���� ������ �����
        err.type = ErrorType::emptyLineFound;  // ������ � ������ �������
        err.errorLineNumber = lineNumber;  // ��������� ������ � �������
        err.errorLineContent = "";  // ������ ������
        err.errorMessage = "���������� ������ ������ �� ������� ������. ������� ������ ������.";  // ��������� �� ������
        return false;  // ���������� false
    }
    return true;  // ������ �� �����
}

// ��������, ��� � ������ ����� ���� ������ ';'
bool FileParser::checkInvalidElementCount(const std::string& line, Error& err, int lineNumber) {
    size_t countSemicolons = 0;  // ������� ����� � �������
    for (char c : line) {
        if (c == ';') ++countSemicolons;  // ����������� �������, ���� ������ ';'
    }

    if (countSemicolons != 1) {  // ���� ����� � ������� ����������� �� ���� ���
        err.type = ErrorType::wrongElementCountInLine;  // ������ � ���������� ���������
        err.errorLineNumber = lineNumber;  // ��������� ������ � �������
        err.errorLineContent = line;  // ���������� ������ � �������
        err.errorMessage = "������������ ���������� ��������� � ������: " + line +
            ". ������ ����� ������ ��������� ��� ����� �����, ���������� ;.";  // ��������� �� ������
        return false;  // ���������� false
    }
    return true;  // ������ ���������
}

// ��������, ��� ��� ������� � ������ ���������
bool FileParser::checkInvalidCharacters(const std::string& line, Error& err, int lineNumber) {
    for (char c : line) {
        if (!(std::isdigit(static_cast<unsigned char>(c)) || c == '-' || c == ';')) {  // �������� �� �����, '-' ��� ';'
            err.type = ErrorType::invalidCharacters;  // ������ � ������������� ���������
            err.errorLineNumber = lineNumber;  // ��������� ������ � �������
            err.errorLineContent = line;  // ���������� ������ � �������
            err.errorMessage = "������� ������ �������� ������������ �������. ��������� ������ �����, ����� � ������� � �������� �����.";  // ��������� �� ������
            return false;  // ���������� false
        }
    }
    return true;  // ��� ������� ���������
}

// ��������, ��� ���������� ����� ������������� � ����� � ��������� ������� ��� �����
bool FileParser::checkNonIntegerCoordinates(const std::string& line, Error& err, int lineNumber) {
    float x, y;
    if (!parsePointLine(line, x, y)) {  // ������ ����������
        err.type = ErrorType::pointNotInteger;  // ������ ��� ��������
        err.errorLineNumber = lineNumber;  // ��������� ������ � �������
        err.errorLineContent = line;  // ���������� ������ � �������
        err.errorMessage = "���������� �� �������� ������ � ��������� ������� ��� ����� ������.";  // ��������� �� ������
        return false;  // ���������� false
    }
    return true;  // ���������� ���������
}

// ������� �������� ������ � �����
bool FileParser::parsePointLine(const std::string& line, float& x, float& y) {
    size_t sep = line.find(';');  // ������� ����������� ';'
    if (sep == std::string::npos) return false;  // ���� �� ����� �����������, ���������� false

    std::string xs = line.substr(0, sep);  // ������ ������ ����� (x)
    std::string ys = line.substr(sep + 1);  // ������ ������ ����� (y)

    if (xs.empty() || ys.empty()) return false;  // ���� ���� �� ���� ����� ������, ���������� false

    try {
        size_t pos;
        float lx = std::stof(xs, &pos);  // ����������� ������ ����� � float
        if (pos != xs.size()) return false;  // ���� �� ��� ������ ���� ������������� � �����, ���������� false
        float ly = std::stof(ys, &pos);  // ����������� ������ ����� � float
        if (pos != ys.size()) return false;  // ���� �� ��� ������ ���� ������������� � �����, ���������� false
        if (lx < -999.0f || lx > 999.0f || ly < -999.0f || ly > 999.0f) return false;  // ��������� ��������
        x = lx;  // ����������� x
        y = ly;  // ����������� y
        return true;  // ���������� true, ���� �� ������ �������
    }
    catch (...) {
        return false;  // ���� ��������� ������, ���������� false
    }
}

// ��������, ��� ���������� ��������� � ���������� ���������
bool FileParser::checkOutOfRangeCoordinates(float x, float y, Error& err, int lineNumber, bool isVertex) {
    if (x < -999.0f || x > 999.0f || y < -999.0f || y > 999.0f) {  // �������� ���������
        if (isVertex) {
            err.type = ErrorType::coordinateOutOfRange;  // ������ � ������������ �������
            err.errorLineNumber = lineNumber;  // ��������� ������ � �������
            err.errorLineContent = std::to_string(x) + ";" + std::to_string(y);  // ���������� ����������
            err.errorMessage = "���������� ������� (" + std::to_string(x) + ";" + std::to_string(y) +
                ") ������� �� ���������� �������� [-999, 999].";  // ��������� �� ������
        }
        else {
            err.type = ErrorType::pointOutOfRange;  // ������ � ������������ �����
            err.errorLineNumber = lineNumber;  // ��������� ������ � �������
            err.errorLineContent = std::to_string(x) + ";" + std::to_string(y);  // ���������� ����������
            err.errorMessage = "����������� ����� (" + std::to_string(x) + ";" + std::to_string(y) +
                ") ������� �� ���������� �������� [-999, 999].";  // ��������� �� ������
        }
        return false;  // ���������� false
    }
    return true;  // ���������� true, ���� �� � �������
}
