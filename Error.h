#pragma once

#include <string>

/// Перечисление возможных типов ошибок
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

/// Класс для хранения информации об ошибке
class Error {
public:
    ErrorType type;
    std::string errorMessage;
    int errorLineNumber;
    std::string errorLineContent;
    std::string errorInputFileWay;
    std::string errorOutputFileWay;

    Error();

    /// Формирует полное описание ошибки
    std::string generateErrorMessage() const;
};

