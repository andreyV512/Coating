// TestStoreBase.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Data/StoreBase.h"
#include "Data/Data.h"

int main()
{
    Initialize initialize;
    auto &rd = Singleton<Data::ResultData>::Instance();
    double *min = rd.minData;
    double *max = rd.maxData;
    unsigned char *st = rd.status;
    rd.count = 99;
    for (int i = 0; i < 99; ++i)
    {
        min[i] = i;
        max[i] = 2.0 * i;
        st[i] = i;
    }
    Store::Result();
    std::cout << "Hello World!\n";
}

