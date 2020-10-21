#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"

MIN_VALUE(CountStoredFiles, 0)
MAX_EQUAL_VALUE(CountStoredFiles, 10000)

PARAM_TITLE(CountStoredFiles, L"Количество сохраненных файлов")
PARAM_TITLE(StoreFileOn, L"Включение сохранения файлов")

void StoreFilesDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, StoreTable
	>(Singleton<StoreTable>::Instance()).Do(h, (wchar_t *)L"Сохранение файлов"))
	{
	}
}


