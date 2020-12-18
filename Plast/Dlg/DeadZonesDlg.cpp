#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/EmptyWindow.h"
#include "Compute/SetTresholds.hpp"
#include "Compute/Compute.h"

MIN_EQUAL_VALUE(DeadZoneStart, 0)
MAX_EQUAL_VALUE(DeadZoneStart, 500)
PARAM_TITLE(DeadZoneStart, L"Неизмеряемая зона начало")

MIN_EQUAL_VALUE(DeadZoneStop, 0)
MAX_EQUAL_VALUE(DeadZoneStop, 500)
PARAM_TITLE(DeadZoneStop, L"Неизмеряемая зона конец")

void DeadZonesDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, DeadZonesTable
	>(Singleton<DeadZonesTable>::Instance()).Do(h, (wchar_t *)L"Неизмеряемые зоны"))
	{
		SetParam(
			Singleton<Compute>::Instance()
			, Singleton<DeadZonesTable>::Instance().items
		);
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
	>(table).Do(h, (wchar_t *)L"Неизмеряемые зоны"))
	{
		VL::CopyFromTo(table.items, w->deadZones);

		SetParam(
			Singleton<Compute>::Instance()
			, table.items
		);

		w->UpdateMedian();
		RepaintWindow(w->hWnd);
	}
}
