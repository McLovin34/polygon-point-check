#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <climits>

bool FileParser::readFromFile(const std::string& fileName,
    std::vector<Point>& vertices,
    Point& testPoint,
    Error& err) {
    // ��������� ������� ���� ��� ������ �� ����� fileName
    std::ifstream fin(fileName);

    // ���������� ���� � �������� ����� � ������ ������ (��� ���������������)
    err.errorInputFileWay = fileName;

    // ���������, ������� �� ������ ����
    if (!fin.is_open()) {
        // ���� ���� �� ������ � ������������� ��� ������ "������� ���� �� ����������"
        err.type = ErrorType::inputFileNotExist;
        // ���������� ��������� ��������� �� ������
        err.errorMessage = "������� ������ ���� � �������� �������. ��������, ���� �� ���������� ��� ��� ���� �� ������.";
        // ���������� false � ���������� ����������
        return false;
    }

    std::string line;   // ������ ��� �������� ������� ������ �����
    int lineNumber = 0; // ������� ������ ������� ������ (��� ���������� �� �������)

    // ������ ������ ������: ���������� ������
    if (!std::getline(fin, line)) {
        // ���� �� ������� ��������� ������ ������ � ���� ����
        err.type = ErrorType::emptyFile;
        err.errorLineNumber = 0;
        err.errorLineContent = "";
        err.errorMessage = "������ ����";
        return false;
    }
    ++lineNumber; // ����������� ����� ������ (������ lineNumber = 1)

    // ��������: �� �������� �� ������ ������ ��� ��������� ������ �� ��������
    if (!checkEmptyLine(line, err, lineNumber)) return false;

    // ������ ������ �� ������ ��������� ������ ';'
    if (line.find(';') != std::string::npos) {
        // ���� ���� ������ �������� � ������ ������� ������ ������
        err.type = ErrorType::wrongElementCountInLine;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "������������ ���������� ��������� � ������: " + line +
            ". ������ ������ ������ ��������� ������ ���������� ������.";
        return false;
    }

    // ������� ������������� ������ � ����� ����� N (���������� ������)
    int N;
    try {
        size_t pos;
        long tmp = std::stol(line, &pos);         // ����������� ������ � long
        if (pos != line.size()) throw std::invalid_argument("not integer"); // ��������, ��� ��� ������ � �����
        N = static_cast<int>(tmp);
    }
    catch (...) {
        // ���� �� ���������� ������������� � ������: �� ����� �����
        err.type = ErrorType::pointNotInteger;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "���������� �� �������� ����� ������. ��������� ������ ����� �����.";
        return false;
    }

    // ���������, ��� N � ��������� [3, 1000]
    if (N < 3 || N > 1000) {
        err.type = ErrorType::invalidVertexCount;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "������������ ���������� ������. ���������� ��������: [3, 1000].";
        return false;
    }

    vertices.clear();      // ������� ������ ������
    vertices.reserve(N);   // ����������� ������ ��� N ������

    // ������ N ����� � ��������� ��������
    for (int i = 0; i < N; ++i) {
        if (!std::getline(fin, line)) {
            // ���� ����� � ��������� ������, ��� ����� � ������: �� ������� ���������
            err.type = ErrorType::verticesMismatch;
            err.errorLineNumber = lineNumber + 1;
            err.errorLineContent = "";
            err.errorMessage = "���������� ��������������: ����� ������ �� ��������� � ����������� �������� ���������. �������� ��� ������� ����������.";
            return false;
        }
        ++lineNumber; // ��������� � ��������� ������

        // ��������: ������ �� �����
        if (!checkEmptyLine(line, err, lineNumber)) return false;
        // ��������: ���������� ��������� � ������ (��������, �� ������ ���� ���������)
        if (!checkInvalidElementCount(line, err, lineNumber)) return false;
        // ��������: ��� �� ������������ ��������
        if (!checkInvalidCharacters(line, err, lineNumber)) return false;

        int x, y;
        // ������ ���������� �� ������ (������ ���� ��� ����� �����)
        if (!parsePointLine(line, x, y)) {
            // ���� �� ������� ���������� � ������: �� ����� �����
            err.type = ErrorType::pointNotInteger;
            err.errorLineNumber = lineNumber;
            err.errorLineContent = line;
            err.errorMessage = "���������� �� �������� ����� ������. ��������� ������ ����� �����.";
            return false;
        }

        // ���������, ��� ���������� �� ������� �� ���������� ��������
        if (!checkOutOfRangeCoordinates(x, y, err, lineNumber, true)) return false;

        // ��������� ������� � ������
        vertices.emplace_back(x, y);
    }

    // ������ ��������� ������: ���������� ����������� �����
    if (!std::getline(fin, line)) {
        // ���� ��� ������ ��� ����� � ������: �� ������� ������
        err.type = ErrorType::verticesMismatch;
        err.errorLineNumber = lineNumber + 1;
        err.errorLineContent = "";
        err.errorMessage = "���������� ��������������: ����� ������ �� ��������� � ����������� �������� ���������. �������� ��� ������� ����������.";
        return false;
    }
    ++lineNumber; // ������� �����

    // ���������, ��� ������ �� ������
    if (!checkEmptyLine(line, err, lineNumber)) return false;
    // ���������, ��� ���������� ��������� � ������ ���������� (������ ��� ����������)
    if (!checkInvalidElementCount(line, err, lineNumber)) return false;
    // ��������� ���������� ������������ ��������
    if (!checkInvalidCharacters(line, err, lineNumber)) return false;

    int tx, ty;
    // ������ ���������� ����������� �����
    if (!parsePointLine(line, tx, ty)) {
        err.type = ErrorType::pointNotInteger;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = line;
        err.errorMessage = "���������� �� �������� ����� ������. ��������� ������ ����� �����.";
        return false;
    }
    // ��������� �������� ��������� �����
    if (!checkOutOfRangeCoordinates(tx, ty, err, lineNumber, false)) return false;
    // ���������� ����������� �����
    testPoint = Point(tx, ty);

    // �������� �� ������� ������ ����� (��������, ���� ����� ��������� ������ ���� ��� �������� ������)
    if (std::getline(fin, line)) {
        bool onlySpaces = true; // ����: ������ �������� ������ �������/���������
        for (char c : line) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                onlySpaces = false;
                break;
            }
        }
        if (!onlySpaces) {
            // ���� ���� ��� �������������� ������ � ������
            err.type = ErrorType::verticesMismatch;
            err.errorLineNumber = lineNumber + 1;
            err.errorLineContent = line;
            err.errorMessage = "���������� ��������������: �������������� ������ ����� ���������.";
            return false;
        }
    }

    return true; // �� �������
}

bool FileParser::checkEmptyLine(const std::string& line, Error& err, int lineNumber) {
    if (line.empty()) {
        err.type = ErrorType::emptyLineFound;
        err.errorLineNumber = lineNumber;
        err.errorLineContent = "";
        err.errorMessage = "���������� ������ ������ �� ������� ������. ������� ������ ������.";
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
        err.errorMessage = "������������ ���������� ��������� � ������: " + line +
            ". ������ ����� ������ ��������� ��� ����� �����, ���������� ;.";
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
            err.errorMessage = "������� ������ �������� ������������ �������. ��������� ������ �����, ����� � ������� � �������� �����.";
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
        err.errorMessage = "���������� �� �������� ����� ������. ��������� ������ ����� �����.";
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
            err.errorMessage = "���������� ������� (" + std::to_string(x) + ";" + std::to_string(y) +
                ") ������� �� ���������� �������� [-999, 999].";
        }
        else {
            err.type = ErrorType::pointOutOfRange;
            err.errorLineNumber = lineNumber;
            err.errorLineContent = std::to_string(x) + ";" + std::to_string(y);
            err.errorMessage = "����������� ����� (" + std::to_string(x) + ";" + std::to_string(y) +
                ") ������� �� ���������� �������� [-999, 999].";
        }
        return false;
    }
    return true;
}
