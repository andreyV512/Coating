#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"

DO_NOT_CHECK(LirDescriptor)

MIN_EQUAL_VALUE(TickPerMMLir0, 0.01)
MAX_EQUAL_VALUE(TickPerMMLir0, 10)

MIN_EQUAL_VALUE(TickPerMMLir1, 0.01)
MAX_EQUAL_VALUE(TickPerMMLir1, 10)

PARAM_TITLE(LirDescriptor, L"Дескриптор платы")
PARAM_TITLE(TickPerMMLir0, L"Датчик1 (миллиметров на отчёт)")
PARAM_TITLE(TickPerMMLir1, L"Датчик2 (миллиметров на отчёт)")

void LirDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, LirParamTable
	>(Singleton< LirParamTable>::Instance()).Do(h, (wchar_t *)L"Датчик ЛИР"))
	{
	}
}