#pragma once

#include <string>

/// ������������ ��������� ����� ������
enum class ErrorType {
    noError,
    inputFileNotExist,
    outputFileCreateFail,
    invalidVertexCount,
    coordinateOutOfRange,
    pointOutOfRange,
    pointNotInteger,
    duplicateVertex,
    invalidPolygon,
    invalidCharacters,
    wrongElementCountInLine,
    emptyLineFound,
    verticesMismatch,
    wrongOrder,
    emptyFile
};

/// ����� ��� �������� ���������� �� ������
class Error {
public:
    ErrorType type;
    std::string errorMessage;
    int errorLineNumber;
    std::string errorLineContent;
    std::string errorInputFileWay;
    std::string errorOutputFileWay;

    Error();

    /// ��������� ������ �������� ������
    std::string generateErrorMessage() const;
};

