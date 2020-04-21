#include "Dlg.h"
#include "window_tool/OpenWindow.hpp"
#include "Windows/AScanWindow/AScanWindow.h"

void AScanWindowDlg::Do(HWND h)
{
	OpenWindow<AScanWindow>::Do(h);
}
