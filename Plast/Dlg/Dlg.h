#pragma once
#include <windows.h>

#define DLG(name)struct name{static void Do(HWND);};
   DLG(DspFiltrDlg)
   DLG(OperatorsDlg)
   DLG(AboutWindowDlg)
   DLG(LanDlg)
#undef DLG