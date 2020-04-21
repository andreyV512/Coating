#pragma once
#include "Base/Base.hpp"
#include "window_tool/message.h"
#include "window_tool/Pass.h"
#include "tools_debug/DebugMess.h"
#include "ParamDlg.hpp"
//--------------------------------------------------------------------------------------------------------------------------
bool TemplDlg_Do(HWND hWnd, wchar_t *title, DLGPROC proc, LPARAM param);

template<class O, class P>struct __make_btn__
{
	void operator()(O &o, P &p)
	{
		CreateWindow(L"button", o.Title()
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			, p.offs, p.height, O::width, 30, p.h, (HMENU )(LONG_PTR)O::ID, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		p.offs += O::width + 10;
	}
};
template<class O, class P>struct TemplDialogCtlColorEdit 
{
	bool operator()(O *, P *)
	{
		return true;
	}
};
template<class O, class P>struct __command__
{
	bool operator()(O &o, P &p)
	{
		if(p.command.id == O::ID)
		{
			o.BtnHandler(p.owner, p.command.hwnd);
			return false;
		}
		return true;
	}
};

template<class, class Table>struct TestPassword;
template<class Base, class Table, class T>struct __ok_table_btn__;

struct OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return (wchar_t *)L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		__ok_table_btn__<
			Owner::Base, Owner::Table
			, typename VL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
		>x;
		if(TestPassword<Owner::Base, Owner::Table>()(h))
		{
			if(x(h, owner))  
			{
				EndDialog(h, TRUE);
			}
		}
	}
};

struct CancelBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return (wchar_t *)L"Закрыть";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		EndDialog(h, FALSE);
	}
};

template<class T>struct __test_password__;
template<>struct __test_password__<ParametersBase::one_row_table_list>
{
	bool operator()(HWND h)
	{
		return OptionPasswordDlg().Do(h);
	}
};
template<>struct __test_password__<ParametersBase::multy_row_table_list>
{
	bool operator()(HWND h)
	{
		return TypesizePasswordDlg().Do(h);
	}
};

template<class Table>struct TestPassword<ParametersBase, Table>
{
	bool operator()(HWND h)
	{
		typedef typename VL::SubListFromMultyList<ParametersBase::multy_type_list, Table>::Result sub;
		return __test_password__<sub>()(h);
	}
};



