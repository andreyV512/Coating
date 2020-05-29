#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "window_tool/TableIni.hpp"
#include "Windows/ZonesWindow/ZonesAxes.h"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/Emptywindow.h"

MIN_EQUAL_VALUE(AxeYTop, 1)
MAX_EQUAL_VALUE(AxeYTop, 100)

MIN_EQUAL_VALUE(AxeYBottom, -100)
MAX_EQUAL_VALUE(AxeYBottom, 1)

MIN_EQUAL_VALUE(AxeXWidth, 100)
MAX_EQUAL_VALUE(AxeXWidth, 3000)

PARAM_TITLE(AxeYTop, L"Максимальное значение оси Y")
PARAM_TITLE(AxeYBottom, L"Минимальное значение оси Y")
PARAM_TITLE(AxeXWidth, L"Ширина оси Х")

void ZonesAxesDlg::Do(HWND h)
{
	HWND hh = FindWindow(WindowClass<ZonesWindow>()(), 0);
	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(hh, GWLP_USERDATA);
	if (Dialog::Templ<ParametersIni, ZoneWindowsAxes
	>(w->zoneAxes).Do(h, (wchar_t *)L"Оси графика"))
	{
	}
}