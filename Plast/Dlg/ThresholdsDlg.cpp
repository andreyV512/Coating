#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/AScanWindow/AScanWindow.h"
#include "window_tool/Emptywindow.h"
#include "Windows/ZonesWindow/ZonesWindow.h"

typedef GROUP_BOX(
	AlarmThresh
	, AlarmThreshStart
	, AlarmThreshStop
	, AlarmGainStart
	, AlarmGainStop
) GBThresh;
PARAM_TITLE(GBThresh, L"Порог брака")

typedef GROUP_BOX(
	BottomReflectionThresh
	, BottomReflectionThreshStart
	, BottomReflectionThreshStop
	, BottomReflectionGainStart
	, BottomReflectionGainStop
	, BottomReflectionOn
) GBBottomReflection;
PARAM_TITLE(GBBottomReflection, L"Порог донного отражения")

MIN_EQUAL_VALUE(AlarmThresh, 0)
MAX_EQUAL_VALUE(AlarmThresh, 100)

MIN_EQUAL_VALUE(AlarmThreshStart, 0)
MAX_EQUAL_VALUE(AlarmThreshStart, 100)
								
MIN_EQUAL_VALUE(AlarmThreshStop, 0)
MAX_EQUAL_VALUE(AlarmThreshStop, 100)
								
MIN_EQUAL_VALUE(AlarmGainStart, 0.1)
MAX_EQUAL_VALUE(AlarmGainStart, 100)
								
MIN_EQUAL_VALUE(AlarmGainStop, 0.1)
MAX_EQUAL_VALUE(AlarmGainStop, 100)
							
MIN_EQUAL_VALUE(BottomReflectionThresh, 0)
MAX_EQUAL_VALUE(BottomReflectionThresh, 100)
						
MIN_EQUAL_VALUE(BottomReflectionThreshStart, 0)
MAX_EQUAL_VALUE(BottomReflectionThreshStart, 100)
						
MIN_EQUAL_VALUE(BottomReflectionThreshStop, 0)
MAX_EQUAL_VALUE(BottomReflectionThreshStop, 100)
							
MIN_EQUAL_VALUE(BottomReflectionGainStart, 0.1)
MAX_EQUAL_VALUE(BottomReflectionGainStart, 100)
							
MIN_EQUAL_VALUE(BottomReflectionGainStop, 0.1)
MAX_EQUAL_VALUE(BottomReflectionGainStop, 100)

MIN_EQUAL_VALUE(SoundSpeed, 1000)
MAX_EQUAL_VALUE(SoundSpeed, 10000)

PARAM_TITLE(AlarmThresh				 , L"Порог")
PARAM_TITLE(AlarmThreshStart		 , L"Начало %")
PARAM_TITLE(AlarmThreshStop			 , L"Конец %")
PARAM_TITLE(AlarmGainStart			 , L"Усиление начало")
PARAM_TITLE(AlarmGainStop			 , L"Усиление конец")
PARAM_TITLE(BottomReflectionThresh		 , L"Порог")
PARAM_TITLE(BottomReflectionThreshStart	 , L"Начало %")
PARAM_TITLE(BottomReflectionThreshStop	 , L"Конец %")
PARAM_TITLE(BottomReflectionGainStart, L"Усиление начало")
PARAM_TITLE(BottomReflectionGainStop , L"Усиление конец")	   
PARAM_TITLE(BottomReflectionOn, L"Использовать метод контроля")
PARAM_TITLE(SoundSpeed, L"Скорость звука")

void ThresholdsDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection, SoundSpeed>, 300
	>(Singleton<TresholdsTable>::Instance()).Do(h, (wchar_t *)L"Пороги"))
	{
		RepaintWindow<MainWindow>();
	}
}

void ThreshDlg::Do(HWND h)
{
	AScanWindow &w = *(AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	TresholdsTable t;
	VL::CopyFromTo(w.computeFrame.treshItems, t.items);
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection, SoundSpeed>, 300
		, Vlst<NoStoreOkBtn, CancelBtn>
	>(t).Do(h, (wchar_t *)L"Пороги"))
	{
		VL::CopyFromTo(t.items, w.computeFrame.treshItems);
		w.SetThresh();
		w.UpdateOptions();
		RepaintWindow(h);
	}
}

void TestThreshDlg::Do(HWND h)
{
	ZonesWindow &w = *(ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	TresholdsTable t;
	VL::CopyFromTo(w.treshItems, t.items);
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection, SoundSpeed>, 300
		, Vlst<NoStoreOkBtn, CancelBtn>
	>(t).Do(h, (wchar_t *)L"Пороги"))
	{
		VL::CopyFromTo(t.items, w.treshItems);
		w.SetThresh();
		RepaintWindow(h);
	}
}


