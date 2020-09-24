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
	, BottomReflectionOn
) GBBottomReflection;
PARAM_TITLE(GBBottomReflection, L"����� ������� ���������")

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
PARAM_TITLE(BottomReflectionOn, L"������������ ����� ��������")

void ThresholdsDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection>, 300
	>(Singleton<TresholdsTable>::Instance()).Do(h, (wchar_t *)L"������"))
	{
	}
}