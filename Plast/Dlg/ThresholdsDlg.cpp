#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"

typedef GROUP_BOX(
	AlarmThresh
	, AlarmThreshStart
	, AlarmThreshStop
	, AlarmGainStart
	, AlarmGainStop
) GBThresh;
PARAM_TITLE(GBThresh, L"Порог брака")

typedef GROUP_BOX(
	BottomReflection
	, BottomReflectionStart
	, BottomReflectionStop
	, BottomReflectionGainStart
	, BottomReflectionGainStop
) GBBottomReflection;
PARAM_TITLE(GBBottomReflection, L"Порог донного отражения")

MIN_EQUAL_VALUE(AlarmThresh, 10)
MAX_EQUAL_VALUE(AlarmThresh, 1000)

MIN_EQUAL_VALUE(AlarmThreshStart, 20)
MAX_EQUAL_VALUE(AlarmThreshStart, 200)
								
MIN_EQUAL_VALUE(AlarmThreshStop, 200)
MAX_EQUAL_VALUE(AlarmThreshStop, 500)
								
MIN_EQUAL_VALUE(AlarmGainStart, 1)
MAX_EQUAL_VALUE(AlarmGainStart, 10)
								
MIN_EQUAL_VALUE(AlarmGainStop, 1)
MAX_EQUAL_VALUE(AlarmGainStop, 10)
							
MIN_EQUAL_VALUE(BottomReflection, 20)
MAX_EQUAL_VALUE(BottomReflection, 200)
						
MIN_EQUAL_VALUE(BottomReflectionStart, 20)
MAX_EQUAL_VALUE(BottomReflectionStart, 200)
						
MIN_EQUAL_VALUE(BottomReflectionStop, 500)
MAX_EQUAL_VALUE(BottomReflectionStop, 600)
							
MIN_EQUAL_VALUE(BottomReflectionGainStart, 1)
MAX_EQUAL_VALUE(BottomReflectionGainStart, 10)
							
MIN_EQUAL_VALUE(BottomReflectionGainStop, 1)
MAX_EQUAL_VALUE(BottomReflectionGainStop, 10)

PARAM_TITLE(AlarmThresh				 , L"Порог")
PARAM_TITLE(AlarmThreshStart		 , L"Начало")
PARAM_TITLE(AlarmThreshStop			 , L"Конец")
PARAM_TITLE(AlarmGainStart			 , L"Усиление начало")
PARAM_TITLE(AlarmGainStop			 , L"Усиление конец")
PARAM_TITLE(BottomReflection		 , L"Порог")
PARAM_TITLE(BottomReflectionStart	 , L"Начало")
PARAM_TITLE(BottomReflectionStop	 , L"Конец")
PARAM_TITLE(BottomReflectionGainStart, L"Усиление начало")
PARAM_TITLE(BottomReflectionGainStop , L"Усиление конец")	   

void ThresholdsDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection>, 300
	>(Singleton<TresholdsTable>::Instance()).Do(h, (wchar_t *)L"Пороги"))
	{
	}
}