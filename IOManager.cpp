#include "IOManager.h"
#include <fstream>
#include <iostream>

bool IOManager::writeResult(const std::string& fileName, bool result, Error& err) {
    std::ofstream fout(fileName);
    err.errorOutputFileWay = fileName;
    if (!fout.is_open()) {
        err.type = ErrorType::outputFileCreateFail;
        err.errorMessage = "Ќеверно указан файл дл€ выходных данных. ¬озможно, указанного расположени€ не существует или нет прав на запись.";
        return false;
    }
    if (result) {
        fout << "принадлежит\n";
    }
    else {
        fout << "не принадлежит\n";
    }
    fout.close();
    return true;
}

void IOManager::writeErrorToConsole(const Error& err) {
    if (err.type == ErrorType::noError) {
        std::cout << "ќшибок не найдено\n";
    }
    else {
        std::cout << err.generateErrorMessage() << std::endl;
    }
}

