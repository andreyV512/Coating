#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"

MIN_VALUE(MedianFiltreWidth, 0)
MAX_EQUAL_VALUE(MedianFiltreWidth, 15)

PARAM_TITLE(MedianFiltreWidth, L"Ўирина фильтра")
PARAM_TITLE(MedianFiltreON, L"¬ключение фильтра")

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
		wsprintf(&buf[wcslen(buf)], L", либо параметр должен быть нечЄтным");
		MessageBox(h, buf, (wchar_t *)L"ќшибка!!!", MB_ICONEXCLAMATION);
	}
};

void MedianFiltreDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, MedianFiltreTable
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, (wchar_t *)L"ћедианный фильтр"))
	{
	}
}
