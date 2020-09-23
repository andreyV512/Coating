#include <Windows.h>
#include "Dlg.h"
#include "window_tool/Emptywindow.h"
#include "window_tool/MenuAPI.h"
#include "App/AppBase.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "window_tool/Emptywindow.h"
#include "window_tool/WindowsPosition.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "window_tool/TEvent.h"
#include "window_tool/GroupBox.h"
#include "App/App.h"

class TestIOWindow;	
namespace TestIOPortsN
{
	template<class T>struct OutStat;
	template<class T>struct InpStat;

#define INPUT_STAT_TYPES_0\
	iCU\
	, iIn \
	, iOut  \
	, iStrobe  

#define OUTPUT_STAT_TYPES_0\
	oAutomat   \
	, oSupply  \
	, oMark \

#define INPUT_WHAT(n)  DEFINE_PARAM_WAPPER(InpStat, n, bool, false)
	FOR_EACH(INPUT_WHAT, INPUT_STAT_TYPES_0)
#undef INPUT_WHAT

#define DLG_SUB_ITEMS(n)template<>struct DlgSubItems<InpStat<n> , bool>: DlgItemsRadio<InpStat<n>>{};
		FOR_EACH(DLG_SUB_ITEMS, INPUT_STAT_TYPES_0)
#undef DLG_SUB_ITEMS

#define INPUT_WHAT(n)  DEFINE_PARAM_WAPPER(OutStat, n, bool, false)
	FOR_EACH(INPUT_WHAT, OUTPUT_STAT_TYPES_0)
#undef INPUT_WHAT

#undef INPUT_STAT_TYPES_0
#undef OUTPUT_STAT_TYPES_0

		PARAM_TITLE(InpStat<iCU     >, L"Сигнал ЦЕПИ УПРАВЛЕНИЯ")
		PARAM_TITLE(InpStat<iIn >, L"Труба на входе")
		PARAM_TITLE(InpStat<iOut >, L"Труба на выходе")
		PARAM_TITLE(InpStat<iStrobe >, L"Строб")

		typedef GROUP_BOX(
		InpStat<iCU     >
		, InpStat<iIn >
		, InpStat<iOut >
		, InpStat<iStrobe >
		) GBInput;
	PARAM_TITLE(GBInput , L"Вход")

	typedef Vlst<
		GBInput
	> __input_list_0__;

		typedef GROUP_BOX(
		OutStat<oAutomat >
		, OutStat<oSupply  >
		, OutStat<oMark >
		) GBOutput;
		PARAM_TITLE(GBOutput, L"Выход")

		PARAM_TITLE(OutStat<oAutomat >, L"Сигнал \"Автомат\"")
		PARAM_TITLE(OutStat<oSupply  >, L"Сигнал \"Подвод\"")
		PARAM_TITLE(OutStat<oMark >, L"Сигнал \"Отметка\"")

		typedef Vlst<
		GBOutput
		> __output_list_0__;

	template<class T>struct __data_io__
	{
		T &items;
		unsigned msk;
		unsigned &value;
		__data_io__(T &items, unsigned msk, unsigned &value): items(items), msk(msk), value(value){}
	};

	template<class O, class P>struct __i__
	{
		bool operator()(O &o, P &p)
		{
			if(p.msk)
			{
				if(p.msk & o.value)
				{
					p.msk &= ~o.value;
					HWND h = p.items.get<Dialog::DlgItem2<InpStat<O>,NullType>>().hWnd;
					unsigned t = (o.value & p.value) ? BST_CHECKED : BST_UNCHECKED;
					Button_SetCheck(h, t);
				}
				return true;
			}
			return false;
		}
	};	
	template<class O, class P>struct __o__
	{
		bool operator()(O &o, P &p)
		{
			if(p.msk)
			{
				if(p.msk & o.value)
				{					
					p.msk &= ~o.value;
					HWND h = p.items.get<Dialog::DlgItem2<OutStat<O>,NullType>>().hWnd;
					bool b = 0 != (o.value & p.value);
					unsigned t = (b) ? BST_CHECKED : BST_UNCHECKED;
					Button_SetCheck(h, t);
					if(b)
					{
						device1730.WriteOutput(o.value);
					}
					else
					{
						device1730.WriteOutput(0, o.value);
					}
				}
				return true;
			}
			return false;
		}
	};	
}									   

class TestIOWindow					   
{
	template<class O, class P>struct init_check_box
	{
		void operator()(O &o, P &p)
		{
		   SetWindowLongPtr(o.hWnd, GWLP_USERDATA, (LONG_PTR)&p.get<CheckBox<O>>());
		}
	};

	template<class T>struct DisableCheckItem
	{
		template<class Owner>bool operator()(Owner *owner, HWND ho, bool b)
		{
			HWND h = owner->items_output0.get<Dialog::DlgItem2<TestIOPortsN::OutStat<T>, NullType>>().hWnd;
			bool bt = BST_CHECKED == Button_GetCheck(h);
			if(b && bt)
			{
				Button_SetCheck(ho, BST_UNCHECKED);
				return false;
			}
			else
			{
				EnableWindow(h, !b);
				return true;
			}
		}
	};
	template<>struct DisableCheckItem<NullType>
	{
		template<class Owner>bool operator()(Owner *owner, HWND, bool b)
		{
			return true;
		}
	};

	template<class T>class CheckBox: public TEvent
	{
		TestIOWindow *owner;
	public:
		CheckBox(TestIOWindow *owner): owner(owner){}
		void Do(TCommand &l)
		{			
			typedef VL::Inner<VL::Inner<T>::Result>::Result Tbit;
			unsigned bit = Singleton<OutputBitsTable>::Instance().items.get<Tbit>().value;
			bool b = BST_CHECKED == Button_GetCheck(l.hControl);
			if(b)
			{
				device1730.WriteOutput(bit);
			}
			else
			{
				device1730.WriteOutput(0, bit);
			}
		}
	};

	unsigned lastBitsInput, lastBitsOutput;

	typedef VL::TypeToTypeLstParam1<TestIOPortsN::__input_list_0__, Dialog::DlgItem2, NullType>::Result original_list_input0;
	typedef VL::TypeToTypeLstParam1<TestIOPortsN::__output_list_0__, Dialog::DlgItem2, NullType>::Result original_list_output0;
public:
	typedef VL::TypeToTypeLstParam1<Dialog::__del_group_box__<TestIOPortsN::__input_list_0__>::Result, Dialog::DlgItem2, NullType>::Result list_input0;
	typedef VL::TypeToTypeLstParam1<Dialog::__del_group_box__<TestIOPortsN::__output_list_0__>::Result, Dialog::DlgItem2, NullType>::Result list_output0;
private:
	typedef VL::TypeToTypeLst<list_output0, CheckBox>::Result list_check_box;
	VL::Factory<list_check_box> items_check_box;
	int idTimer;
public:
	HWND hWnd;
	bool lastIsRun;
	VL::Factory<list_input0> items_input0;
	VL::Factory<list_output0> items_output0;
	TestIOWindow();
	LRESULT operator()(TCreate &l);
	void operator()(TDestroy &);
	void operator()(TCommand &);
	void operator()(TTimer &);
};

TestIOWindow::TestIOWindow()
	: items_check_box(this)
{}

LRESULT TestIOWindow::operator()(TCreate &l)
{
	lastBitsInput  = ~device1730.Read();
	lastBitsOutput = ~device1730.ReadOutput();
	lastIsRun = !App::IsRun();

	int width = 280;
	int height = 15;
	int xOffs = 5;

	HWND h = GroupBoxWindow((wchar_t *)L"", 3, 5, 565, 208, l.hwnd);

	Dialog::__table_data__X<VL::Factory<list_input0>> data_inp(h, xOffs, width, height, items_input0);
	VL::foreach<original_list_input0, Dialog::__init__X>()(data_inp);
	width = 280;
	height = 15;
	xOffs = 290;
	Dialog::__table_data__X<VL::Factory<list_output0>> data_out(h, xOffs, width, height, items_output0);
	VL::foreach<original_list_output0, Dialog::__init__X>()(data_out);
	VL::foreach<list_output0, init_check_box>()(items_output0, items_check_box);

	SetTimer(l.hwnd, idTimer, 300, NULL);
	return 0;
}

void TestIOWindow::operator()(TDestroy &l)
{	
	KillTimer(l.hwnd, idTimer);
	WindowPosition::Set<TestIOWindow>(l.hwnd);
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	if(!App::IsRun())device1730.Write(0);
	delete this;
}

void TestIOWindow::operator()(TCommand &l)
{
	EventDo(l);
}

namespace
{
	template<class O, class P>struct __enable__
	{
		void operator()(O &o, P &p)
		{
			EnableWindow(o.hWnd, p);
		}
	};
}

void TestIOWindow::operator()(TTimer &)
{
	bool isRun = App::IsRun();
	if(lastIsRun != isRun)
	{
		bool t = !isRun;
		VL::foreach<TestIOWindow::list_output0, __enable__>()(items_output0, t);
		lastIsRun = isRun;
	}
	unsigned test_testIOWindow = device1730.Read();
	TestIOPortsN::__data_io__<VL::Factory<list_input0>> idata(items_input0, test_testIOWindow ^ lastBitsInput, test_testIOWindow);
	VL::find<InputBitsTable::items_list, TestIOPortsN::__i__>()(Singleton<InputBitsTable>::Instance().items, idata);
	lastBitsInput = test_testIOWindow;  
	test_testIOWindow = device1730.ReadOutput();
	TestIOPortsN::__data_io__<VL::Factory<list_output0>> odata(items_output0, test_testIOWindow ^ lastBitsOutput, test_testIOWindow);
	VL::find<OutputBitsTable::items_list, TestIOPortsN::__o__>()(Singleton<OutputBitsTable>::Instance().items, odata);
	lastBitsOutput = test_testIOWindow; 	
}

void TestIOPorts::Do(HWND)
{
	HWND h = FindWindow(WindowClass<TestIOWindow>()(), 0);
	if(NULL != h)
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		RECT r;
		WindowPosition::Get<TestIOWindow>(r);
		const int width = 590;
		const int height = 160;
		h = WindowTemplate(
			new TestIOWindow
			, (wchar_t *)L"Порты ввода-вывода"
			, r.left, r.top
			, width, height
			, IDI_settings
			, (HBRUSH)COLOR_WINDOW
			);

		LONG_PTR dwStyle = GetWindowLongPtr(h, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX|WS_THICKFRAME);
		SetWindowLongPtr(h, GWL_STYLE, dwStyle);
		ShowWindow(h, SW_SHOWNORMAL);

		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void TestIOPortWindowEnable(bool b)
{
	HWND h = FindWindow(WindowClass<TestIOWindow>()(), 0);
	if(NULL != h)
	{
		TestIOWindow &o = *(TestIOWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		VL::foreach<TestIOWindow::list_output0, __enable__>()(o.items_output0, b);
	}
}