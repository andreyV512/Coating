// TestStrobes.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "App/App.h"
#include "Compute/Compute.h"
#include "Data/Data.h"
#include "templates/typelist.hpp"

int main()
{
    Data::InputData &d = Singleton<Data::InputData>::Instance();
    d.offsetsLir[0] = 10;
    d.offsetsLir[1] = 12;
    d.offsetsLir[2] = 14;
    d.offsetsLir[3] = 27;
    d.offsetsLir[4] = 29;
    d.offsetsLir[5] = 31;
    d.offsetsLir[6] = 33;
    d.offsetsLir[7] = 39;
    d.offsetsLir[8] = 52;
    d.offsetsLir[9] = 53;
    d.offsetsLir[10] = 54;
    d.offsetsLir[11] = 64;
    d.offsetsLir[12] = 70;

    d.offsetsLir[13] = 70;
    d.offsetsLir[14] = 72;
    d.offsetsLir[15] = 74;
    d.offsetsLir[16] = 87;
    d.offsetsLir[17] = 89;
    d.offsetsLir[18] = 91;
    d.offsetsLir[19] = 93;
    d.offsetsLir[20] = 99;
    d.offsetsLir[21] = 102;
    d.offsetsLir[22] = 103;
    d.offsetsLir[23] = 104;
    d.offsetsLir[24] = 114;
    d.offsetsLir[25] = 120;

    for (int i = 0; i < dimention_of(d.offsetFrames); ++i)
    {
        d.offsetFrames[i] = (int)d.offsetsLir[i];
    }

    d.startOffsetLir = 19;
    d.countFrames = 0;

    Compute compute;

    int o = 0;
    for (; o <= 25; ++o)
    {
        if (compute.StartStrobes())break;
        ++d.countFrames;
    }

    d.countFrames = 26;
   // for (; o <= 25; ++o)
    {
        compute.Strobes();
    }

    for (int i = 0; i < compute.countZones; ++i)
    {
        printf("%d %d\n", i, compute.zoneOffsets[i]);
    }

    std::cout << "Stop\n";
}
