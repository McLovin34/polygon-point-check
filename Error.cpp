#include "Error.h"
#include <sstream>

// Конструктор по умолчанию для класса Error
Error::Error()
    : type(ErrorType::noError),       // Инициализация типа ошибки как "нет ошибки"
    errorMessage(""),                 // Пустое сообщение об ошибке
    errorLineNumber(0),               // Номер строки с ошибкой по умолчанию 0
    errorLineContent(""),             // Пустое содержимое строки с ошибкой
    errorInputFileWay(""),            // Пустой путь к входному файлу
    errorOutputFileWay("") {          // Пустой путь к выходному файлу
}

// Метод генерации итогового сообщения об ошибке
std::string Error::generateErrorMessage() const {
    std::ostringstream oss; // Поток для формирования итоговой строки

    // Конвертация ErrorType в строковое представление
    std::string typeStr;    // Строка для хранения текстового представления типа ошибки
    switch (type) {         // Определяем строку в зависимости от значения type
    case ErrorType::noError:                typeStr = "noError"; break;                  // Нет ошибки
    case ErrorType::inputFileNotExist:      typeStr = "inputFileNotExist"; break;        // Входной файл не найден
    case ErrorType::outputFileCreateFail:   typeStr = "outputFileCreateFail"; break;     // Не удалось создать выходной файл
    case ErrorType::invalidVertexCount:     typeStr = "invalidVertexCount"; break;       // Некорректное количество вершин
    case ErrorType::coordinateOutOfRange:   typeStr = "coordinateOutOfRange"; break;     // Координата вне допустимого диапазона
    case ErrorType::pointOutOfRange:        typeStr = "pointOutOfRange"; break;          // Точка вне диапазона
    case ErrorType::pointNotInteger:        typeStr = "pointNotInteger"; break;          // Точка не целое число
    case ErrorType::duplicateVertex:        typeStr = "duplicateVertex"; break;          // Дублирующаяся вершина
    case ErrorType::invalidPolygon:         typeStr = "invalidPolygon"; break;           // Некорректный многоугольник
    case ErrorType::invalidCharacters:      typeStr = "invalidCharacters"; break;        // Недопустимые символы
    case ErrorType::wrongElementCountInLine:typeStr = "wrongElementCountInLine"; break;  // Неверное число элементов в строке
    case ErrorType::emptyLineFound:         typeStr = "emptyLineFound"; break;           // Найдена пустая строка
    case ErrorType::verticesMismatch:       typeStr = "verticesMismatch"; break;         // Несоответствие количества вершин
    case ErrorType::wrongOrder:             typeStr = "wrongOrder"; break;               // Неверный порядок
    case ErrorType::emptyFile:              typeStr = "emptyFile"; break;                // Файл пуст
    default:                                typeStr = "unknownError"; break;             // Неизвестная ошибка
    }

    oss << "Ошибка: " << typeStr;                  // Записываем в поток тип ошибки

    if (!errorMessage.empty()) {                   // Если есть дополнительное сообщение
        oss << ": " << errorMessage;               // Добавляем его к сообщению об ошибке
    }
    if (!errorLineContent.empty() || errorLineNumber > 0) { // Если есть содержимое строки или номер строки
        oss << "; Строка: \"" << errorLineContent << "\"; Линия: " << errorLineNumber;
    }
    if (!errorInputFileWay.empty()) {              // Если указан путь к входному файлу
        oss << "; Файл: " << errorInputFileWay;
    }
    if (!errorOutputFileWay.empty()) {             // Если указан путь к выходному файлу
        oss << "; Вывод: " << errorOutputFileWay;
    }
    return oss.str();                              // Возвращаем итоговое сообщение об ошибке
}
