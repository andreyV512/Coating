#include "Dlg.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/AScanWindow/AScanWindow.h"
#include "window_tool/Emptywindow.h"

typedef GROUP_BOX(
	AlarmThresh
	, AlarmThreshStart
	, AlarmThreshStop
	, AlarmGainStart
	, AlarmGainStop
) GBThresh;
PARAM_TITLE(GBThresh, L"����� �����")

typedef GROUP_BOX(
	BottomReflectionThresh
	, BottomReflectionThreshStart
	, BottomReflectionThreshStop
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

PARAM_TITLE(AlarmThresh				 , L"�����")
PARAM_TITLE(AlarmThreshStart		 , L"������")
PARAM_TITLE(AlarmThreshStop			 , L"�����")
PARAM_TITLE(AlarmGainStart			 , L"�������� ������")
PARAM_TITLE(AlarmGainStop			 , L"�������� �����")
PARAM_TITLE(BottomReflectionThresh		 , L"�����")
PARAM_TITLE(BottomReflectionThreshStart	 , L"������")
PARAM_TITLE(BottomReflectionThreshStop	 , L"�����")
PARAM_TITLE(BottomReflectionGainStart, L"�������� ������")
PARAM_TITLE(BottomReflectionGainStop , L"�������� �����")	   
PARAM_TITLE(BottomReflectionOn, L"������������ ����� ��������")

void ThresholdsDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection>, 300
	>(Singleton<TresholdsTable>::Instance()).Do(h, (wchar_t *)L"������"))
	{
		RepaintWindow<MainWindow>();
	}
}

template<class O, class P>struct __thresh_ok_btn__
{
	void operator()(O &o, P &p)
	{
		typedef typename VL::Inner<O>::Result inner;
		auto &x = p.get<inner>();
		x.value = __data_from_widget__<O, inner::type_value>()(o);
	}
};

struct ThreshOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title() { return (wchar_t *)L"���������"; }
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		HWND hw = FindWindow(WindowClass<AScanWindow>()(), 0);
		AScanWindow &w = *(AScanWindow *)GetWindowLongPtr(hw, GWLP_USERDATA);
		if (!VL::find<Owner::list, __test__>()(owner.items, h))return;
		VL::foreach<Owner::list, __thresh_ok_btn__>()(owner.items, w.treshItems);
		EndDialog(h, TRUE);
	}
};

void ThreshDlg::Do(HWND h)
{
	HWND hw = FindWindow(WindowClass<AScanWindow>()(), 0);
	AScanWindow &w = *(AScanWindow *)GetWindowLongPtr(hw, GWLP_USERDATA);
	TresholdsTable t;
	VL::CopyFromTo(w.treshItems, t.items);
	if (Dialog::Templ<ParametersBase, TresholdsTable, Vlst<GBThresh, GBBottomReflection>, 300
		, Vlst<ThreshOkBtn, CancelBtn>
	>(t).Do(h, (wchar_t *)L"������"))
	{
		w.SetThresh();
		RepaintWindow<AScanWindow>();
	}
}