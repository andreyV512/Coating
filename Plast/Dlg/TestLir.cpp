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
#include "Units/Lir/Lir.h"
#include "templates/templates.hpp"

class TestLirWindow
{
	int idTimer;
	struct ResetButton1 : TEvent
	{
		TestLirWindow &owner;
		ResetButton1(TestLirWindow &owner) : owner(owner) {}
		void Do(TCommand &);
	} resetButton1;

	struct ResetButton2 : TEvent
	{
		TestLirWindow &owner;
		ResetButton2(TestLirWindow &owner) : owner(owner) {}
		void Do(TCommand &);
	} resetButton2;
public:
	Lir &lir = Singleton<Lir>::Instance();
	double &dx0 = Singleton<LirParamTable>::Instance().items.get<TickPerMMLir0>().value;
	double &dx1 = Singleton<LirParamTable>::Instance().items.get<TickPerMMLir1>().value;
	HWND hWnd, he1, he2, hb1, hb2, hmm;
	bool lastIsRun;
	TestLirWindow();
	LRESULT operator()(TCreate &l);
	void operator()(TDestroy &);
	void operator()(TCommand &);
	void operator()(TTimer &);
};

TestLirWindow::TestLirWindow()
	: resetButton1(*this)
	, resetButton2(*this)
{ }

void TestLirWindow::ResetButton1::Do(TCommand &l)
{
	owner.lir.Clear1();
}

void TestLirWindow::ResetButton2::Do(TCommand &l)
{
	owner.lir.Clear2();
}

LRESULT TestLirWindow::operator()(TCreate &l)
{
	he1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"000"
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
		, 10, 15, 80, 25, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);
	CreateWindow(L"static", L"Датчик1"
		, WS_VISIBLE | WS_CHILD
		, 95, 20, 60, 25, l.hwnd, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);
	hb1 = CreateWindow(L"button", L"Сброс"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		, 160, 15, 80, 25, l.hwnd, NULL, GetModuleHandle(NULL), NULL
	);
	SetWindowLongPtr(hb1, GWLP_USERDATA, (LONG_PTR)&resetButton1);

	int dy = 30;

	he2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"000"
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
		, 10, 15 + dy, 80, 25, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);
	CreateWindow(L"static", L"Датчик2"
		, WS_VISIBLE | WS_CHILD
		, 95, 20 + dy, 60, 25, l.hwnd, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);
	hb2 = CreateWindow(L"button", L"Сброс"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		, 160, 15 + dy, 80, 25, l.hwnd, NULL, GetModuleHandle(NULL), NULL
	);
	SetWindowLongPtr(hb2, GWLP_USERDATA, (LONG_PTR)&resetButton2);
 //----------------------------------------------------
	dy += 30;
	hmm = CreateWindow(L"button", L"Миллиметры"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 10, 15 + dy, 110, 25, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);
	Button_SetCheck(hmm, BST_CHECKED);
	dy += 20;
	CreateWindow(L"button", L"Отчёты"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 10, 15 + dy, 110, 25, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);

	SetTimer(l.hwnd, idTimer, 300, NULL);
	return 0;
}

void TestLirWindow::operator()(TDestroy &l)
{
	KillTimer(l.hwnd, idTimer);
	WindowPosition::Set<TestLirWindow>(l.hwnd);
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	delete this;
}

void TestLirWindow::operator()(TCommand &l)
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

void TestLirWindow::operator()(TTimer &)
{
	bool isRun = App::IsRun();
	if (lastIsRun != isRun)
	{
		bool t = !isRun;
		//VL::foreach<TestLirWindow::list_output0, __enable__>()(items_output0, t);
		EnableWindow(hb1, t);
		EnableWindow(hb2, t);
		lastIsRun = isRun;
	}
	unsigned t1 = lir.Value1();
	unsigned t2 = lir.Value2();

	if (BST_CHECKED == Button_GetCheck(hmm))
	{
		double x1 = dx0 * t1;
		double x2 = dx1 * t2;
		SetWindowText(he1, Wchar_from<double, 1>(x1)());
		SetWindowText(he2, Wchar_from<double, 1>(x2)());
	}
	else
	{
		wchar_t buf[64];
		_itow_s(t1, buf, 10);
		SetWindowText(he1, buf);
		_itow_s(t2, buf, 10);
		SetWindowText(he2, buf);
	}
}

void TestLirDlg::Do(HWND)
{
	HWND h = FindWindow(WindowClass<TestLirWindow>()(), 0);
	if (NULL != h)
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		RECT r;
		WindowPosition::Get<TestLirWindow>(r);
		const int width = 280;
		const int height = 170;
		h = WindowTemplate(
			new TestLirWindow
			, (wchar_t *)L"Тест ЛИР"
			, r.left, r.top
			, width, height
			, IDI_settings
			, (HBRUSH)COLOR_WINDOW
		);

		DWORD dwStyle = GetWindowLongPtr(h, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME);
		SetWindowLongPtr(h, GWL_STYLE, dwStyle);
		ShowWindow(h, SW_SHOWNORMAL);

		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}