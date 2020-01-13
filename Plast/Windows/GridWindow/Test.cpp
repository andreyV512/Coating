#include "Test.h"
#include "App/AppBase.h"
#include "Grid/DataGrid.hpp"

HEADER_TABLE(Num, 60, L"�����")
PARAM_TABLE(UserName, 90, L"��������")
PARAM_TABLE(UserPersonnelNumber, 120, L"��������� �����")

CHECK_EMPTY_STRING(UserName)
MIN_VALUE(UserPersonnelNumber, 0)
MAX_VALUE(UserPersonnelNumber, 9999999)


void TestTest()
{
	GridOptions<TDataGrid<UserTable, Vlst<UserName>> >::Open((wchar_t *)L"��������� ���������", 320, 300);
}
