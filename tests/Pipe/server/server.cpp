#include <windows.h>
#include <conio.h>

const wchar_t *ready_data = L"ready_data";

int main()
{

    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, ready_data);
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

    if (!CreateProcess(
        NULL
        , buf
        , NULL
        , NULL
        , TRUE
        , CREATE_NEW_CONSOLE
        , NULL
        , NULL
        , &si
        , &pi
    ))
    {
       
        DWORD ret = GetLastError();
        _cprintf("Create process failed  %x\n", ret);
        _getch();
        return ret;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

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

