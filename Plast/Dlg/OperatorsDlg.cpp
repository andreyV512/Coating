#include "Dlg.h"
#include "App/AppBase.h"
#include "Grid/DataGrid.hpp"

HEADER_TABLE(Num, 60, L"Номер")
PARAM_TABLE(UserName, 90, L"Оператор")
PARAM_TABLE(UserPersonnelNumber, 120, L"Табельный номер")

CHECK_EMPTY_STRING(UserName)
MIN_VALUE(UserPersonnelNumber, 0)
MAX_VALUE(UserPersonnelNumber, 9999999)

template<>void UpdateId<ID<UserTable>>(CBase &base, int num)
{
	CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
	current.items.get<CurrentUserNameID>().value = num;
	Update< CurrentParametersTable>(base).set<CurrentUserNameID>(num).Execute();

	wchar_t name[64];
	App::CurrentOperatorName(name);
	App::StatusBar(App::operator_status_section, name);
}

template<> int CurrentId<ID<UserTable>>()
{
	return Singleton<CurrentParametersTable>::Instance().items.get<CurrentUserNameID>().value;
}

void OperatorsDlg::Do(HWND)
{
	GridOptions<TDataGrid<UserTable, Vlst<UserName>> >::Open((wchar_t *)L"Настройки оператора", 320, 300);
}
