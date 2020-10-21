// testZip.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "window_tool/Zip.h"
//
#pragma comment(lib, "Shlwapi.lib")

int main()
{
   // Zip::ZipAsyncAll();
   // Zip::Unzip((wchar_t *)L"z.dat.bz2");
   // int count = Zip::CountZipFilesInDirectory();
    Zip::RemoveZipFilesInDirectory(2);
    //std::cout << "Hello World!\n" << count;
    getchar();
}

