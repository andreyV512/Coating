#include "Test.h"
#include "App/AppBase.h"
#include "Grid/GridDetail.h"
#include "DataGrid.hpp"

#define Z(T, w, txt)template<>struct GridDetail::header_table<T>\
	{\
	LPWSTR name(){return (LPWSTR)txt;}\
	static const int width = w;\
	};

Z(Num, 60, L"�����")
Z(UserName, 90, L"���")
Z(UserPersonnelNumber, 120, L"��������� �����")
#undef Z


void TestTest()
{
	GridOptions<TDataGrid<UserTable, UserName> >::Open((wchar_t *)L"Test", 320, 300);
}
