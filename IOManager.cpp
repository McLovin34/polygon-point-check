#include "IOManager.h"
#include <fstream>
#include <iostream>

bool IOManager::writeResult(const std::string& fileName, bool result, Error& err) {
    std::ofstream fout(fileName);  // ��������� ���� ��� ������
    err.errorOutputFileWay = fileName;  // ��������� ���� � ����� � ������� ������

    // ���������, ��� �� ���� ������� ������
    if (!fout.is_open()) {
        err.type = ErrorType::outputFileCreateFail;  // ������, ���� ���� �� ��������
        err.errorMessage = "������� ������ ���� ��� �������� ������. ��������, ���������� ������������ �� ���������� ��� ��� ���� �� ������.";  // ��������� �� ������
        return false;  // ���������� false, ���� ���� �� ��������
    }

    // ���������� ��������� � ���� � ����������� �� �������� ���������� result
    if (result) {
        fout << "�����������\n";  // ���� ��������� true, ���������� "�����������"
    }
    else {
        fout << "�� �����������\n";  // ���� ��������� false, ���������� "�� �����������"
    }

    fout.close();  // ��������� ���� ����� ������
    return true;  // ���������� true, ���� �� ������ �������
}

void IOManager::writeErrorToConsole(const Error& err) {
    if (err.type == ErrorType::noError) {
        std::cout << "������ �� �������\n";
    }
    else {
        std::cout << err.generateErrorMessage() << std::endl;
    }
}

