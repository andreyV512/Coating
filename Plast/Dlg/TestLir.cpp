#include "Dlg.h"
#include "window_tool/Emptywindow.h"
#include "window_tool/MenuAPI.h"
#include "App/AppBase.h"
#include "window_tool/WindowsPosition.h"
#include "Units/Lir/Lir.h"

class TestLirWindow
{
	int idTimer;
	void(TestLirWindow::*ptrProc)();
	struct ResetBtn1 : TEvent
	{
		TestLirWindow &owner;
		ResetBtn1(TestLirWindow &owner) : owner(owner) {}
		void Do(TCommand &);
	} resetBtn1;

	struct ResetBtn2 : TEvent
	{
		TestLirWindow &owner;
		ResetBtn2(TestLirWindow &owner) : owner(owner) {}
		void Do(TCommand &);
	} resetBtn2;

	struct RadioBtn1 : TEvent
	{
		TestLirWindow &owner;
		RadioBtn1(TestLirWindow &owner) : owner(owner) {}
		void Do(TCommand &);
	} radioBtn1;
	struct RadioBtn2 : TEvent
	{
		TestLirWindow &owner;
		RadioBtn2(TestLirWindow &owner) : owner(owner) {}
		void Do(TCommand &);
	} radioBtn2;
public:
	Lir &lir = Singleton<Lir>::Instance();
	double &dx0 = Singleton<LirParamTable>::Instance().items.get<TickPerMMLir0>().value;
	double &dx1 = Singleton<LirParamTable>::Instance().items.get<TickPerMMLir1>().value;
	HWND hWnd, he1, he2, hb1, hb2, hmm;
	bool lastIsRun = false;
	TestLirWindow();
	LRESULT operator()(TCreate &l);
	void operator()(TDestroy &);
	void operator()(TCommand &);
	void operator()(TTimer &);

	void ValMM();
	void ValTick();
};

TestLirWindow::TestLirWindow()
	: resetBtn1(*this)
	, resetBtn2(*this)
	, radioBtn1(*this)
	, radioBtn2(*this)
	, ptrProc(&TestLirWindow::ValMM)
{ }

void TestLirWindow::ResetBtn1::Do(TCommand &l)
{
	owner.lir.Clear1();
}

void TestLirWindow::ResetBtn2::Do(TCommand &l)
{
	owner.lir.Clear2();
}

void TestLirWindow::RadioBtn1::Do(TCommand &l)
{
	if (BST_CHECKED == Button_GetCheck(l.hControl))
	{
		owner.ptrProc = &TestLirWindow::ValMM;
	}
}

void TestLirWindow::RadioBtn2::Do(TCommand &l)
{
	if (BST_CHECKED == Button_GetCheck(l.hControl))
	{
		owner.ptrProc = &TestLirWindow::ValTick;
	}
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
	SetWindowLongPtr(hb1, GWLP_USERDATA, (LONG_PTR)&resetBtn1);

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
	SetWindowLongPtr(hb2, GWLP_USERDATA, (LONG_PTR)&resetBtn2);
 //----------------------------------------------------
	dy += 30;
	hmm = CreateWindow(L"button", L"Миллиметры"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 10, 15 + dy, 110, 25, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);
	Button_SetCheck(hmm, BST_CHECKED);
	SetWindowLongPtr(hmm, GWLP_USERDATA, (LONG_PTR)&radioBtn1);
	dy += 20;
	HWND h = CreateWindow(L"button", L"Отчёты"
		, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
		, 10, 15 + dy, 110, 25, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
	);
	SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)&radioBtn2);

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

void TestLirWindow::operator()(TTimer &)
{
	bool isRun = App::IsRun();
	if (lastIsRun != isRun)
	{
		bool t = !isRun;
		EnableWindow(hb1, t);
		EnableWindow(hb2, t);
		lastIsRun = isRun;
	}

	(this->*ptrProc)();
}

void TestLirWindow::ValMM()
{
	unsigned t1 = lir.Value1();
	unsigned t2 = lir.Value2();

	double x1 = dx0 * t1;
	double x2 = dx1 * t2;
	SetWindowText(he1, Wchar_from<double, 1>(x1)());
	SetWindowText(he2, Wchar_from<double, 1>(x2)());
}
void TestLirWindow::ValTick()
{
	unsigned t1 = lir.Value1();
	unsigned t2 = lir.Value2();

	wchar_t buf[64];
	_itow_s(t1, buf, 10);
	SetWindowText(he1, buf);
	_itow_s(t2, buf, 10);
	SetWindowText(he2, buf);
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

		LONG_PTR dwStyle = GetWindowLongPtr(h, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME);
		SetWindowLongPtr(h, GWL_STYLE, dwStyle);
		ShowWindow(h, SW_SHOWNORMAL);

		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}