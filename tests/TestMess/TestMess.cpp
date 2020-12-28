#include <iostream>
#include <vector>
#include <algorithm>

struct TBuf
{
    static const int size = 1024;
    struct Item
    {
        unsigned id, time;
        double data;
    };
    Item items[size];
    unsigned head, tail;
};

struct Display
{
    unsigned id, time;
    double data;
};

static const int display_size = 10;
Display display[display_size];

struct Test
{
    std::vector<int> buf;
    Test()
    {
        buf.push_back(2);
        buf.push_back(3);
        buf.push_back(4);
    }
    bool operator()(int value)
    {
        return  buf.end() != std::find(buf.begin(), buf.end(), value);
    }
};


int main()
{
    Test test;
    TBuf buf;
    buf.tail = 0;
    for (int i = 0; i < TBuf::size; ++i)
    {
        buf.items[i].id = i % 5;
        buf.items[i].data = i;
        buf.items[i].time = i * 100;
        ++buf.head;
    }
    std::cout << "Hello World!\n";

    unsigned k = 0;
    for (int i = 0; i < display_size; ++i)
    {
        while (!test(buf.items[k % TBuf::size].id)) ++k;
        int start = k;
        ++k;
        while (!test(buf.items[k % TBuf::size].id)) ++k;
        int stop = k;
        printf("%d id %d  time %d\n", i, buf.items[start].id, buf.items[stop].time - buf.items[start].time);
    }
}

