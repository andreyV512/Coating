#include <windows.h>
#include <conio.h>
//#include "window_tool\RunExecute.h"
#include "H:\projects\Test\Coating\Common\window_tool\RunExecute.h"

int main()
{
    HANDLE hReadPipe, hWritePipe, hInheritWritePipe;

    if (!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0))
    {
        _cputs("Create pipe failed\n");
        _getch();
        return GetLastError();
    }

    if (!DuplicateHandle(
        GetCurrentProcess()
        , hWritePipe
        , GetCurrentProcess()
        , &hInheritWritePipe
        , 0
        , TRUE
        , DUPLICATE_SAME_ACCESS
    ))
    {
        _cputs("Duplicate handle  failed\n");
        _getch();
        return GetLastError();
    }
    CloseHandle(hWritePipe);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(si);
    wchar_t buf[180];
    wsprintf(buf, L"H:\\projects\\Test\\Coating\\Debug\\client.exe %d", (int)hInheritWritePipe);

    if (!RunExecute(buf))
    {
        DWORD ret = GetLastError();
        _cprintf("Create process failed  %x\n", ret);
        _getch();
        return ret;
    }

    CloseHandle(hInheritWritePipe);
    char buffer[4058];
    DWORD bytesReaded;
    while (true)
    {
      //  WaitForSingleObject(hEvent, INFINITE);
        if (!ReadFile(
            hReadPipe
            , buffer
            , 4058
            , &bytesReaded
            , NULL
        ))
        {
            _cputs("Read from the pipe failed\n");
            _getch();
            return GetLastError();
        }
        _cprintf("number %d bytes readed %d\n", *(int *)buffer, bytesReaded);
    }
    
    CloseHandle(hReadPipe);
    _cputs("Process finished\n");
    _getch();
    return 0;
}

