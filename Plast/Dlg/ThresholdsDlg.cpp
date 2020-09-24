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
							
MIN_EQUAL_VALUE(BottomReflection, 0)
MAX_EQUAL_VALUE(BottomReflection, 100)
						
MIN_EQUAL_VALUE(BottomReflectionStart, 0)
MAX_EQUAL_VALUE(BottomReflectionStart, 100)
						
MIN_EQUAL_VALUE(BottomReflectionStop, 0)
MAX_EQUAL_VALUE(BottomReflectionStop, 100)
							
MIN_EQUAL_VALUE(BottomReflectionGainStart, 0.1)
MAX_EQUAL_VALUE(BottomReflectionGainStart, 100)
							
MIN_EQUAL_VALUE(BottomReflectionGainStop, 0.1)
MAX_EQUAL_VALUE(BottomReflectionGainStop, 100)

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
PARAM_TITLE(BottomReflectionOn, L"Использовать метод контроля")

void ThresholdsDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection>, 300
	>(Singleton<TresholdsTable>::Instance()).Do(h, (wchar_t *)L"Пороги"))
	{
	}
}