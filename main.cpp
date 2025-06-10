#include <iostream>
#include <cstdlib>    // system()
#include <clocale>    // setlocale()
#include <vector>
#include <string>

#include "Error.h"
#include "FileParser.h"
#include "Validator.h"
#include "Polygon.h"
#include "IOManager.h"

int main(int argc, char* argv[]) {
    // Переключаем консоль Windows в кодировку UTF-8, чтобы корректно выводить символы
#ifdef _WIN32
    system("chcp 65001 > nul");  // Команда для Windows, меняем кодовую страницу на UTF-8
#endif

    // Устанавливаем локаль для корректного вывода символов в зависимости от системных настроек
    std::setlocale(LC_ALL, "");  // Устанавливаем локаль из окружения

    std::string inputPath = "input.txt";
    std::string outputPath = "output.txt";

    if (argc == 2) {
        // polygon.exe <input>
        inputPath = argv[1];
    }
    else if (argc == 3) {
        // polygon.exe <input> <output>
        inputPath = argv[1];
        outputPath = argv[2];
    }
    else if (argc > 3) {
        std::cerr << "Ошибка: слишком много аргументов.\n"
            << "Использование:\n"
            << "  polygon.exe             (использует input.txt→output.txt)\n"
            << "  polygon.exe <in>\n"
            << "  polygon.exe <in> <out>\n";
        return 1;
    }
    // если argc==1 — остаются input.txt и

    // 1) Синтаксическое чтение
    FileParser parser;
    std::vector<Point> vertices;
    Point testPoint;
    Error err;

    if (!parser.readFromFile(inputPath, vertices, testPoint, err)) {
        IOManager::writeErrorToConsole(err);
        return 2;
    }

    // 2) Семантическая валидация
    Validator validator;
    if (!validator.validate(vertices, testPoint, err)) {
        IOManager::writeErrorToConsole(err);
        return 3;
    }

    // 3) Проверка корректности многоугольника
    Polygon polygon(vertices);
    if (!polygon.isValid(err)) {
        IOManager::writeErrorToConsole(err);
        return 4;
    }

    // 4) Проверка принадлежности точки
    bool belongs = polygon.contains(testPoint);

    // 5) Запись результата
    if (!IOManager::writeResult(outputPath, belongs, err)) {
        IOManager::writeErrorToConsole(err);
        return 5;
    }

    // Печатать окончательный результат в консоль
    std::cout << (belongs ? "принадлежит" : "не принадлежит") << std::endl;
    return 0;
}