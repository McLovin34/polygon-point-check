#pragma once

#include "Error.h"
#include <string>

/// Класс IOManager содержит статические методы для записи результата и вывода ошибок в консоль.
class IOManager {
public:
    /// \brief Записывает результат (принадлежит/не принадлежит) в выходной файл.
    /// \param[in]   fileName – путь к выходному файлу.
    /// \param[in]   result   – true → "принадлежит", false → "не принадлежит".
    /// \param[out]  err      – объект Error для записи ошибок открытия/записи.
    /// \return true, если запись успешна; false — если файл не открылся.
    static bool writeResult(const std::string& fileName, bool result, Error& err);

    /// \brief Выводит в консоль сообщение об ошибке из объекта Error.
    ///        Если err.type == noError, печатает "Ошибок не найдено".
    static void writeErrorToConsole(const Error& err);
};

