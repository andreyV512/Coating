#pragma once
#include <windows.h>

#define DLG(name)struct name{static void Do(HWND);};
   DLG(DspFiltrDlg)
   DLG(OperatorsDlg)
   DLG(AboutWindowDlg)
   DLG(LanDlg)
   DLG(TestIOPorts)
   DLG(AScanWindowDlg)
   DLG(InputBitDlg)
   DLG(OutputBitDlg)
   DLG(DiscriptorBitDlg)
#undef DLG