#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/EmptyWindow.h"
#include "SensItem.hpp"
#include "Compute/SetTresholds.hpp"
#include "Compute/Compute.h"

XMIN_VALUE      (MedianFiltreWidth, 0)
XMAX_EQUAL_VALUE(MedianFiltreWidth, 15)

template<int N>using GBMedian = Dialog::GroupBox<typename GB<Vlst<
	MedianFiltreWidth
	, MedianFiltreON
>, N>::Result>;

PARAM_TITLE(GBMedian<0>, L"Медианный фильтр 1")
PARAM_TITLE(GBMedian<1>, L"Медианный фильтр 2")
PARAM_TITLE(GBMedian<2>, L"Медианный фильтр 3")

XPARAM_TITLE(MedianFiltreWidth, L"Ширина фильтра")
XPARAM_TITLE(MedianFiltreON, L"Включение фильтра")

template<int N>struct __compare_param__<Less<Num<MedianFiltreWidth, N>>, LargenEqual<Num<MedianFiltreWidth, N>>, Num<MedianFiltreWidth, N>>
{
	typedef Num<MedianFiltreWidth, N> T;
	bool operator()(typename T::type_value &t)
	{
		return t >= Less<T>()() && t <= LargenEqual<T>()() && 0 != (t & 1);
	}
};

template<int N>struct ErrMess<Num<MedianFiltreWidth, N>>
{
	typedef Num<MedianFiltreWidth, N> T;
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
		, Vlst<GBMedian<0>, GBMedian<1>, GBMedian<2>>
		, 310
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, (wchar_t *)L"Медианные фильтры"))
	{
		SetParam(
			Singleton<Compute>::Instance()
			, Singleton<MedianFiltreTable>::Instance().items
		);
	}
}

void TstMedianFiltreDlg::Do(HWND h)
{
	MedianFiltreTable table;

	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	VL::CopyFromTo(w->medianItems, table.items);
	if (Dialog::Templ<ParametersBase, MedianFiltreTable
		, Vlst<GBMedian<0>, GBMedian<1>, GBMedian<2>>
		, 310
		, Vlst<NoStoreOkBtn, CancelBtn>
	>(table).Do(h, (wchar_t *)L"Медианные фильтры"))
	{
		VL::CopyFromTo(table.items, w->medianItems);
		w->UpdateMedian();

		SetParam(
			//Singleton<Compute>::Instance()
			w->computeZone
			, w->medianItems
		);

		w->UpdateZone();
	}
}
