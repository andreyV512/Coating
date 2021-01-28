#include "Dlg.h"
#include "window_tool/MenuApi.h"
#include "Windows/AScanWindow/AScanWindow.h"
#include "window_tool/Emptywindow.h"
#include "Windows/AScanWindow/AScanWindowMenu.hpp"
#include "Windows/ZonesWindow/ZonesWindowMenu.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"

void BiPolarDlg::Do(HWND h)
{
	bool b = SwitchMenu<MenuItem<AScanWindowMenu::BiPolar>>(h);
	AScanWindow *w = (AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);

	w->SwitchBipolar(b);
	RepaintWindow(h);
}

void TestBiPolarDlg::Do(HWND h)
{
	bool b = SwitchMenu<MenuItem<ZonesWindowMenu::BiPolar>>(h);
	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);

	w->SwitchBipolar(b);
	RepaintWindow(h);
}

void XinMMDlg::Do(HWND h)
{
	AScanWindow *w = (AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	w->XinMM = SwitchMenu<MenuItem<AScanWindowMenu::XinMM__>>(h);
	RepaintWindow(h);
}

void TestXinMMDlg::Do(HWND h)
{
	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	w->XinMM = SwitchMenu<MenuItem<ZonesWindowMenu::XinMM__>>(h);
	RepaintWindow(h);
}

void GainDlg::Do(HWND h)
{
	AScanWindow *w = (AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	bool b = SwitchMenu<MenuItem<AScanWindowMenu::Gain__>>(h);
	w->GainEnable(b);
	RepaintWindow(h);
}
void TestGainDlg::Do(HWND h)
{
	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	bool b = SwitchMenu<MenuItem<ZonesWindowMenu::Gain__>>(h);
	w->GainEnable(b);
	RepaintWindow(h);
}
void flt_dlg(bool b, HWND h)
{
	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	CheckMenu<MenuItem<ZonesWindowMenu::FiltreAmpl>>(h, b);
	CheckMenu<MenuItem<ZonesWindowMenu::FiltreStat>>(h, !b);
	w->amplFilt = b;
	w->UpdateMedian();
	w->UpdateZone();
	w->UpdateAScan();
}
void FiltreAmplDlg::Do(HWND h)
{
	flt_dlg(true, h);
}

void FiltreStatDlg::Do(HWND h)
{
	flt_dlg(false, h);
}