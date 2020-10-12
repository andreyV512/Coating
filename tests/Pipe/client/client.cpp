#include <iostream>
#include <windows.h>
#include <conio.h>

const wchar_t *ready_data = L"ready_data";

unsigned k = 99;
char buf[4058];

int main(int argc, char **argv)
{
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, ready_data);
    HANDLE hWritePipe = (HANDLE)atoi(argv[1]);

    DWORD bytesWritten;
    while (true)
    {
        *(unsigned *)buf = k;

        if (!WriteFile(hWritePipe, buf, 4058, &bytesWritten, NULL))
        {
            DWORD ret = GetLastError();
            _cprintf("client WriteFalted %d\n", ret);
            _getch();
            return ret;
        }
     //   SetEvent(hEvent);
        _cprintf("client number %d bytes written %d\n", k, bytesWritten);
    //    Sleep(100);
        ++k;
    }
    CloseHandle(hWritePipe);
    _cputs("Press any key\n");
    _getch();
}

