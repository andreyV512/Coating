#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/AScanWindow/AScanWindow.h"
#include "window_tool/Emptywindow.h"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "SensItem.hpp"
#include "Compute/SetTresholds.hpp"
#include "Compute/Compute.h"
#include "Data/StoreAllParam.h"

template<int N>using GBThresh = Dialog::GroupBox<typename GB<Vlst<
	AlarmThresh
	, AlarmThreshStart
	, AlarmThreshStop
	, AlarmGainStart
	, AlarmGainStop
>, N>::Result>;

PARAM_TITLE(GBThresh<0>, L"Порог брака")
PARAM_TITLE(GBThresh<1>, L"Порог брака")
PARAM_TITLE(GBThresh<2>, L"Порог брака")

template<int N>using GBBottomReflection = Dialog::GroupBox<typename GB<Vlst<
	BottomReflectionThresh
	, BottomReflectionThreshStart
	, BottomReflectionThreshStop
	, BottomReflectionGainStart
	, BottomReflectionGainStop
	, BottomReflectionOn
>, N>::Result>;

PARAM_TITLE(GBBottomReflection<0>, L"Порог донного отражения")
PARAM_TITLE(GBBottomReflection<1>, L"Порог донного отражения")
PARAM_TITLE(GBBottomReflection<2>, L"Порог донного отражения")

XMIN_EQUAL_VALUE(AlarmThresh, 0)
XMAX_EQUAL_VALUE(AlarmThresh, 100)

XMIN_EQUAL_VALUE(AlarmThreshStart, 0)
XMAX_EQUAL_VALUE(AlarmThreshStart, 100)
							
XMIN_EQUAL_VALUE(AlarmThreshStop, 0)
XMAX_EQUAL_VALUE(AlarmThreshStop, 100)
							
XMIN_EQUAL_VALUE(AlarmGainStart, 0.1)
XMAX_EQUAL_VALUE(AlarmGainStart, 100)
								
XMIN_EQUAL_VALUE(AlarmGainStop, 0.1)
XMAX_EQUAL_VALUE(AlarmGainStop, 100)
							
XMIN_EQUAL_VALUE(BottomReflectionThresh, 0)
XMAX_EQUAL_VALUE(BottomReflectionThresh, 100)
						
XMIN_EQUAL_VALUE(BottomReflectionThreshStart, 0)
XMAX_EQUAL_VALUE(BottomReflectionThreshStart, 100)
					
XMIN_EQUAL_VALUE(BottomReflectionThreshStop, 0)
XMAX_EQUAL_VALUE(BottomReflectionThreshStop, 100)
						
XMIN_EQUAL_VALUE(BottomReflectionGainStart, 0.1)
XMAX_EQUAL_VALUE(BottomReflectionGainStart, 100)
						
XMIN_EQUAL_VALUE(BottomReflectionGainStop, 0.1)
XMAX_EQUAL_VALUE(BottomReflectionGainStop, 100)

MIN_EQUAL_VALUE(SoundSpeed, 1000)
MAX_EQUAL_VALUE(SoundSpeed, 10000)

XPARAM_TITLE(AlarmThresh				 , L"Порог")
XPARAM_TITLE(AlarmThreshStart		 , L"Начало %")
XPARAM_TITLE(AlarmThreshStop			 , L"Конец %")
XPARAM_TITLE(AlarmGainStart			 , L"Усиление начало")
XPARAM_TITLE(AlarmGainStop			 , L"Усиление конец")
XPARAM_TITLE(BottomReflectionThresh		 , L"Порог")
XPARAM_TITLE(BottomReflectionThreshStart	 , L"Начало %")
XPARAM_TITLE(BottomReflectionThreshStop	 , L"Конец %")
XPARAM_TITLE(BottomReflectionGainStart, L"Усиление начало")
XPARAM_TITLE(BottomReflectionGainStop , L"Усиление конец")	   
XPARAM_TITLE(BottomReflectionOn, L"Использовать метод контроля")

PARAM_TITLE(SoundSpeed, L"Скорость звука")
//////////////////////////////////////////////////////////////
template<class Base>struct __ok_table_btn__<Base, TresholdsTable, typename Base::multy_row_table_list>
	: _tpl_ok_table_btn__<Base, TresholdsTable, typename Base::multy_row_table_list> {};

	template<class List, int N>struct __sel_num__;
	template<int N, class Head, class ...Tail>struct __sel_num__<Vlst<Head, Tail...>, N>
	{
		typedef typename __sel_num__<Vlst<Tail...>, N>::Result Result;
	};
	template<int N, class Head, class ...Tail>struct __sel_num__<Vlst<Num<Head, N>, Tail...>, N>
	{
		typedef typename VL::Append<Num<Head, N>, typename __sel_num__<Vlst<Tail...>, N>::Result>::Result Result;
	};
	template<int N>struct __sel_num__<Vlst<>, N> { typedef Vlst<> Result; };
template<class O, class P>struct __curr_sens_XXX__
{
	bool operator()(P &p)
	{
		static const int N = O::value;
		if (N == p.currentSensor)
		{
			typedef Vlst<CurrentSensor, GBThresh<N>, GBBottomReflection<N>, SoundSpeed> list;
			p.close = true;
			wchar_t buf[128];
			wsprintf(buf, L"Пороги. Датчик %d", 1 + N);
			
			if (Dialog::Templ<ParametersBase, TresholdsTable
				, list
				, 300
				, Vlst<OkBtn, CancelBtn, Dialog::NoButton<CurrentSensor>>
				, __current_filtre_param_data__<TresholdsTable>
			>(p.table, &p).Do(p.h, buf))
			{
				typedef typename __sel_num__<TresholdsTable::items_list, N>::Result _list;
				typedef typename VL::Append<SoundSpeed, _list>::Result list;
				p.Restore<list>();

				SetParam(
					Singleton<Compute>::Instance()
					, p.items
				);

				ALLPatramsUpdate<TresholdsTable::items_list, TresholdsTable>();

				RepaintWindow<MainWindow>();
			}
			return false;
		}
		return true;
	}
};
static int currentSensor = 0;
void ThresholdsDlg::Do(HWND h)
{
	__current_filtre_param_data__<TresholdsTable> data(h, Singleton<TresholdsTable>::Instance().items, currentSensor);
	while (!data.close) VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __curr_sens_XXX__>()(data);
}

template<class O, class P>struct __curr_sens_NoStoreOkBtn__
{
	bool operator()(P &p)
	{
		static const int N = O::value;
		if (N == p.currentSensor)
		{
			typedef Vlst<CurrentSensor, GBThresh<N>, GBBottomReflection<N>, SoundSpeed> list;
			p.close = true;
			wchar_t buf[128];
			wsprintf(buf, L"Пороги. Датчик %d", 1 + N);

			if (Dialog::Templ<ParametersBase, TresholdsTable
				, list
				, 300
				, Vlst<NoStoreOkBtn, CancelBtn, Dialog::NoButton<CurrentSensor>>
				, __current_filtre_param_data__<TresholdsTable>
			>(p.table, &p).Do(p.h, buf))
			{
				typedef typename __sel_num__<TresholdsTable::items_list, N>::Result _list;
				typedef typename VL::Append<SoundSpeed, _list>::Result list;

				SetParam(
					Singleton<Compute>::Instance()
					, p.items
				);

				p.Restore<list>();				
			}
			return false;
		}
		return true;
	}
};

void ThreshDlg::Do(HWND h)
{
	AScanWindow &w = *(AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	__current_filtre_param_data__<TresholdsTable> data(h, w.computeFrame.treshItems, currentSensor);
	while (!data.close) VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __curr_sens_NoStoreOkBtn__>()(data);

	w.UpdateOptions();
	w.SetThresh();
	RepaintWindow(h);
}

void TestThreshDlg::Do(HWND h)
{
	ZonesWindow &w = *(ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	__current_filtre_param_data__<TresholdsTable> data(h, w.treshItems, currentSensor);
	while (!data.close) VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __curr_sens_NoStoreOkBtn__>()(data);

	w.SetThresh();
	w.UpdateZone();
	w.UpdateAScan();
}


