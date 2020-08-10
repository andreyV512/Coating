#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"

MIN_EQUAL_VALUE(OffsetSensorBegMM, 0)
MAX_EQUAL_VALUE(OffsetSensorBegMM, 100)

MIN_EQUAL_VALUE(OffsetSensorEndMM, 0)
MAX_EQUAL_VALUE(OffsetSensorEndMM, 100)

PARAM_TITLE(OffsetSensorBegMM, L"�������� ������� ������")
PARAM_TITLE(OffsetSensorEndMM, L"�������� ������� �����")

void UnitDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, UnitTable
	>(Singleton<UnitTable>::Instance()).Do(h, (wchar_t *)L"�������� �������"))
	{
	}
}