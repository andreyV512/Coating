#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"

MIN_VALUE(MedianFiltreWidth, 0)
MAX_EQUAL_VALUE(MedianFiltreWidth, 15)

PARAM_TITLE(MedianFiltreWidth, L"������ �������")
PARAM_TITLE(MedianFiltreON, L"��������� �������")

template<>struct __compare_param__<Less<MedianFiltreWidth>, LargenEqual<MedianFiltreWidth>, MedianFiltreWidth>
{
	typedef MedianFiltreWidth T;
	bool operator()(typename T::type_value &t)
	{
		return t >= Less<T>()() && t <= LargenEqual<T>()() && 0 != (t & 1);
	}
};

template<>struct ErrMess<MedianFiltreWidth>
{
	typedef MedianFiltreWidth T;
	void operator()(typename T::type_value &t, HWND h)
	{
		wchar_t buf[512];
		typedef VL::TypeExist<Vlst<Less<T>, LessEqual<T> > >::Result min_type;
		typedef VL::TypeExist< Vlst<Largen<T>, LargenEqual<T> > >::Result max_type;
		SubErrMess<min_type, max_type, T>()(buf);
		wsprintf(&buf[wcslen(buf)], L", ���� �������� ������ ���� ��������");
		MessageBox(h, buf, (wchar_t *)L"������!!!", MB_ICONEXCLAMATION);
	}
};

void MedianFiltreDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, MedianFiltreTable
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, (wchar_t *)L"��������� ������"))
	{
	}
}
