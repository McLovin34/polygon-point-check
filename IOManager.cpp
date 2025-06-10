#include "IOManager.h"
#include <fstream>
#include <iostream>

bool IOManager::writeResult(const std::string& fileName, bool result, Error& err) {
    std::ofstream fout(fileName);  // Открываем файл для записи
    err.errorOutputFileWay = fileName;  // Указываем путь к файлу в объекте ошибки

    // Проверяем, был ли файл успешно открыт
    if (!fout.is_open()) {
        err.type = ErrorType::outputFileCreateFail;  // Ошибка, если файл не открылся
        err.errorMessage = "Неверно указан файл для выходных данных. Возможно, указанного расположения не существует или нет прав на запись.";  // Сообщение об ошибке
        return false;  // Возвращаем false, если файл не открылся
    }

    // Записываем результат в файл в зависимости от значения переменной result
    if (result) {
        fout << "принадлежит\n";  // Если результат true, записываем "принадлежит"
    }
    else {
        fout << "не принадлежит\n";  // Если результат false, записываем "не принадлежит"
    }

    fout.close();  // Закрываем файл после записи
    return true;  // Возвращаем true, если всё прошло успешно
}

void IOManager::writeErrorToConsole(const Error& err) {
    if (err.type == ErrorType::noError) {
        std::cout << "Ошибок не найдено\n";
    }
    else {
        std::cout << err.generateErrorMessage() << std::endl;
    }
}

