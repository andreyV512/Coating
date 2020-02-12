#pragma once
#include <windows.h>

#define DLG(name)struct name{static void Do(HWND);};
   DLG(DspFiltrDlg)
#undef DLG