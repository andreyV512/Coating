#include "SelectUserDlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "App/AppBase.h"

PARAM_TITLE(UserName, L"Оператор")
PARAM_TITLE(UserPersonnelNumber, L"Табельный номер")

CHECK_EMPTY_STRING(UserName)
MIN_VALUE(UserPersonnelNumber, 0)
MAX_VALUE(UserPersonnelNumber, 9999999)

struct sel_OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title() { return (wchar_t *)L"Применить"; }
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if (__ok_table_btn__<
			Owner::Base, Owner::Table
			, typename VL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
		>()(h, owner))
		{
			EndDialog(h, TRUE);
		}
	}
};

bool SelectUserDlg::Do(HWND h, UserTable &t)
{
	return Dialog::Templ<
		ParametersBase
		, UserTable
		, UserTable::items_list
		, 300
		, Vlst<sel_OkBtn, CancelBtn>
	>(t).Do(h, (wchar_t *)L"Настройки оператора");
}
