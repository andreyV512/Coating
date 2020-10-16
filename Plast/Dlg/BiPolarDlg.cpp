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