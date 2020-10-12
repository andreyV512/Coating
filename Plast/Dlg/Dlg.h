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
   DLG(AddTypeSizeDlg)
   DLG(DelTypeSizeDlg)
   DLG(MessageDlg)
   DLG(LirDlg)
   DLG(TestLirDlg)
   DLG(UnitDlg)
   DLG(TstDspFiltrDlg)
 //  DLG(ZonesAxesDlg)
   DLG(MedianFiltreDlg)
   DLG(ThresholdsDlg)
   DLG(ColorDlg)
   DLG(ThreshDlg)
   DLG(BiPolarDlg)
   DLG(AScanDspFiltrDlg)
#undef DLG