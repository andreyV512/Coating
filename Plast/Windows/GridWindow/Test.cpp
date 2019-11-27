#include "Test.h"
#include "App/AppBase.h"
#include "Grid/GridDetail.h"
#include "DataGrid.hpp"



HEADER_TABLE(Num, 60, L"�����")
HEADER_TABLE(UserName, 90, L"���")
HEADER_TABLE(UserPersonnelNumber, 120, L"��������� �����")

PARAM_TITLE(UserName, L"��������")
PARAM_TITLE(UserPersonnelNumber, L"��������� �����")

CHECK_EMPTY_STRING(UserName)
MIN_VALUE(UserPersonnelNumber, 0)
MAX_VALUE(UserPersonnelNumber, 9999999)


void TestTest()
{
	GridOptions<TDataGrid<UserTable, UserName> >::Open((wchar_t *)L"��������� ���������", 320, 300);
}
