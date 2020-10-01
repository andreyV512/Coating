#include "Dlg.h"
#include "window_tool/MenuApi.h"
#include "Windows/AScanWindow/AScanWindow.h"
#include "window_tool/Emptywindow.h"
#include "Windows/AScanWindow/AScanWindowMenu.hpp"

void BiPolarDlg::Do(HWND h)
{
	bool b = SwitchMenu<MenuItem<AScanWindowMenu::BiPolar>>(h);
	AScanWindow *w = (AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);

	w->SwitchBipolar(b);
	RepaintWindow(h);
}