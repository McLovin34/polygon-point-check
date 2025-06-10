#include "Error.h"
#include <sstream>

Error::Error()
    : type(ErrorType::noError),
    errorMessage(""),
    errorLineNumber(0),
    errorLineContent(""),
    errorInputFileWay(""),
    errorOutputFileWay("") {
}

std::string Error::generateErrorMessage() const {
    std::ostringstream oss;
    // Конвертация ErrorType в строковое представление
    std::string typeStr;
    switch (type) {
    case ErrorType::noError:                typeStr = "noError"; break;
    case ErrorType::inputFileNotExist:      typeStr = "inputFileNotExist"; break;
    case ErrorType::outputFileCreateFail:   typeStr = "outputFileCreateFail"; break;
    case ErrorType::invalidVertexCount:     typeStr = "invalidVertexCount"; break;
    case ErrorType::coordinateOutOfRange:   typeStr = "coordinateOutOfRange"; break;
    case ErrorType::pointOutOfRange:        typeStr = "pointOutOfRange"; break;
    case ErrorType::pointNotInteger:        typeStr = "pointNotInteger"; break;
    case ErrorType::duplicateVertex:        typeStr = "duplicateVertex"; break;
    case ErrorType::invalidPolygon:         typeStr = "invalidPolygon"; break;
    case ErrorType::invalidCharacters:      typeStr = "invalidCharacters"; break;
    case ErrorType::wrongElementCountInLine:typeStr = "wrongElementCountInLine"; break;
    case ErrorType::emptyLineFound:         typeStr = "emptyLineFound"; break;
    case ErrorType::verticesMismatch:       typeStr = "verticesMismatch"; break;
    case ErrorType::wrongOrder:             typeStr = "wrongOrder"; break;
    case ErrorType::emptyFile:              typeStr = "emptyFile"; break;
    default:                                 typeStr = "unknownError"; break;
    }

    oss << "Ошибка: " << typeStr;
    if (!errorMessage.empty()) {
        oss << ": " << errorMessage;
    }
    if (!errorLineContent.empty() || errorLineNumber > 0) {
        oss << "; Строка: \"" << errorLineContent << "\"; Линия: " << errorLineNumber;
    }
    if (!errorInputFileWay.empty()) {
        oss << "; Файл: " << errorInputFileWay;
    }
    if (!errorOutputFileWay.empty()) {
        oss << "; Вывод: " << errorOutputFileWay;
    }
    return oss.str();
}
