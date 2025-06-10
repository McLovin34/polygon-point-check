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

    std::string inputPath = "input.txt";  // Задаём путь к файлу входных данных по умолчанию
    std::string outputPath = "output.txt";  // Задаём путь к файлу выходных данных по умолчанию

    // Обработка аргументов командной строки
    if (argc == 2) {
        // Если указан только один аргумент (путь к входному файлу)
        // polygon.exe <input>
        inputPath = argv[1];  // Присваиваем путь к входному файлу
    }
    else if (argc == 3) {
        // Если указаны два аргумента (входной и выходной файл)
        // polygon.exe <input> <output>
        inputPath = argv[1];  // Присваиваем путь к входному файлу
        outputPath = argv[2];  // Присваиваем путь к выходному файлу
    }
    else if (argc > 3) {
        // Если аргументов больше двух — выводим сообщение об ошибке
        std::cerr << "Ошибка: слишком много аргументов.\n"
            << "Использование:\n"
            << "  polygon.exe             (использует input.txt→output.txt)\n"
            << "  polygon.exe <in>\n"
            << "  polygon.exe <in> <out>\n";  // Сообщаем правильное использование программы
        return 1;  // Завершаем программу с кодом ошибки 1
    }
    // если argc==1 — остаются input.txt и output.txt

// 1) Синтаксическое чтение данных из файла
    FileParser parser;  // Создаём объект для чтения данных из файла
    std::vector<Point> vertices;  // Вектор для хранения вершин многоугольника
    Point testPoint;  // Точка для проверки принадлежности
    Error err;  // Объект для хранения ошибок

    // Попытка прочитать данные из файла
    if (!parser.readFromFile(inputPath, vertices, testPoint, err)) {
        IOManager::writeErrorToConsole(err);  // Если произошла ошибка, выводим её в консоль
        return 2;  // Завершаем программу с кодом ошибки 2
    }

    // 2) Семантическая валидация данных
    Validator validator;  // Создаём объект для валидации данных
    // Проверка, что данные о многоугольнике и точке корректны
    if (!validator.validate(vertices, testPoint, err)) {
        IOManager::writeErrorToConsole(err);  // Если данные некорректны, выводим ошибку в консоль
        return 3;  // Завершаем программу с кодом ошибки 3
    }

    // 3) Проверка корректности многоугольника
    Polygon polygon(vertices);  // Создаём объект многоугольника, передавая вектор вершин
    // Проверяем, является ли многоугольник корректным (проверка на выпуклость, отсутствие самопересечений и т.д.)
    if (!polygon.isValid(err)) {
        IOManager::writeErrorToConsole(err);  // Если многоугольник некорректен, выводим ошибку в консоль
        return 4;  // Завершаем программу с кодом ошибки 4
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