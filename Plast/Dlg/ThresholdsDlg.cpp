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
PARAM_TITLE(GBThresh, L"����� �����")

typedef GROUP_BOX(
	BottomReflection
	, BottomReflectionStart
	, BottomReflectionStop
	, BottomReflectionGainStart
	, BottomReflectionGainStop
) GBBottomReflection;
PARAM_TITLE(GBBottomReflection, L"����� ������� ���������")

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

PARAM_TITLE(AlarmThresh				 , L"�����")
PARAM_TITLE(AlarmThreshStart		 , L"������")
PARAM_TITLE(AlarmThreshStop			 , L"�����")
PARAM_TITLE(AlarmGainStart			 , L"�������� ������")
PARAM_TITLE(AlarmGainStop			 , L"�������� �����")
PARAM_TITLE(BottomReflection		 , L"�����")
PARAM_TITLE(BottomReflectionStart	 , L"������")
PARAM_TITLE(BottomReflectionStop	 , L"�����")
PARAM_TITLE(BottomReflectionGainStart, L"�������� ������")
PARAM_TITLE(BottomReflectionGainStop , L"�������� �����")	   

void ThresholdsDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection>, 300
	>(Singleton<TresholdsTable>::Instance()).Do(h, (wchar_t *)L"������"))
	{
	}
}