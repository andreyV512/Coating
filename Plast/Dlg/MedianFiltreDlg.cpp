#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/EmptyWindow.h"

MIN_VALUE(MedianFiltreWidth, 0)
MAX_EQUAL_VALUE(MedianFiltreWidth, 15)

PARAM_TITLE(MedianFiltreWidth, L"Ширина фильтра")
PARAM_TITLE(MedianFiltreON, L"Включение фильтра")

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
		wsprintf(&buf[wcslen(buf)], L", либо параметр должен быть нечётным");
		MessageBox(h, buf, (wchar_t *)L"Ошибка!!!", MB_ICONEXCLAMATION);
	}
};

void MedianFiltreDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, MedianFiltreTable
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, (wchar_t *)L"Медианный фильтр"))
	{
	}
}

void TstMedianFiltreDlg::Do(HWND h)
{
	MedianFiltreTable table;

	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	VL::CopyFromTo(w->median, table.items);
	if (Dialog::Templ<ParametersBase, MedianFiltreTable
		, MedianFiltreTable::items_list
		, 550
		, Vlst<NoStoreOkBtn, CancelBtn>
	>(table).Do(h, (wchar_t *)L"Медианный фильтр"))
	{
		VL::CopyFromTo(table.items, w->median);
		w->UpdateMedian();
		RepaintWindow(w->hWnd);
	}
}
