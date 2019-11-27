#include "Test.h"
#include "App/AppBase.h"
#include "Grid/GridDetail.h"
#include "DataGrid.hpp"



HEADER_TABLE(Num, 60, L"Номер")
HEADER_TABLE(UserName, 90, L"Имя")
HEADER_TABLE(UserPersonnelNumber, 120, L"Табельный номер")

PARAM_TITLE(UserName, L"Оператор")
PARAM_TITLE(UserPersonnelNumber, L"Табельный номер")

CHECK_EMPTY_STRING(UserName)
MIN_VALUE(UserPersonnelNumber, 0)
MAX_VALUE(UserPersonnelNumber, 9999999)


void TestTest()
{
	GridOptions<TDataGrid<UserTable, UserName> >::Open((wchar_t *)L"Настройки оператора", 320, 300);
}
