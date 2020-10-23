#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/EmptyWindow.h"

MIN_EQUAL_VALUE(DeadZoneStart, 0)
MAX_EQUAL_VALUE(DeadZoneStart, 500)
PARAM_TITLE(DeadZoneStart, L"������������ ���� ������")

MIN_EQUAL_VALUE(DeadZoneStop, 0)
MAX_EQUAL_VALUE(DeadZoneStop, 500)
PARAM_TITLE(DeadZoneStop, L"������������ ���� �����")

void DeadZonesDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, DeadZonesTable
	>(Singleton<DeadZonesTable>::Instance()).Do(h, (wchar_t *)L"������������ ����"))
	{
	}
}

void TestDeadZonesDlg::Do(HWND h)
{
	DeadZonesTable table;
	
	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	VL::CopyFromTo(w->deadZones, table.items);
	if (Dialog::Templ<ParametersBase, DeadZonesTable
		, DeadZonesTable::items_list
		, 550
		, Vlst<NoStoreOkBtn, CancelBtn>
	>(table).Do(h, (wchar_t *)L"������������ ����"))
	{
		VL::CopyFromTo(table.items, w->deadZones);
		w->UpdateMedian();
		RepaintWindow(w->hWnd);
	}
}
