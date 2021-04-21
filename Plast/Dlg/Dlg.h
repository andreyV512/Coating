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
   DLG(MedianFiltreDlg)
   DLG(ThresholdsDlg)
   DLG(ColorDlg)
   DLG(ThreshDlg)
   DLG(BiPolarDlg)
   DLG(AScanDspFiltrDlg)
   DLG(TstMedianFiltreDlg)
   DLG(TestThreshDlg)
   DLG(TestBiPolarDlg)
   DLG(DeadZonesDlg)
   DLG(TestDeadZonesDlg)
   DLG(StoreFilesDlg)
   DLG(OpenFileDlg)
   DLG(SaveFileDlg)
   DLG(XinMMDlg)
   DLG(TestXinMMDlg)
   DLG(GainDlg)
   DLG(TestGainDlg)

   DLG(FiltreAmplDlg)
   DLG(FiltreStatDlg)

   DLG(ComPortDlg)

#undef DLG