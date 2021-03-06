#include "AScanWindow.h"
#include <CommCtrl.h>
#include "AScanWindowMenu.hpp"
#include "window_tool/MenuAPI.h"
#include <window_tool/Emptywindow.h>
#include "window_tool/Common.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "AScanKeyHandler.h"
#include "Windows/AScanWindow/AScanWindow.h"
#include "window_tool/Pass.h"
#include "Devices/LanDevice.h"
#include "Windows/Viewers/NegThresh.hpp"
#include "Windows/StoreParamsBase.hpp"
#include "Compute/SetTresholds.hpp"
#include "Data/StoreAllParam.h"

template<class O, class P>struct __enable_gain__
{
	void operator()(O &o, P &p)
	{
		o.gainLine.enable = p;
	}
};

void AScanWindow::GainEnable(bool b)
{
	VL::foreach<viewers_list, __enable_gain__>()(viewers, b);
}

LRESULT AScanWindow::operator()(TCreate &l)
{
	generatorBit = Singleton<OutputBitsTable>::Instance().items.get<oGenerator>().value;
	SetThresh();

	AppKeyHandler::DisableAll();
	Menu<AScanWindowMenu::Menu>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	CreateChildWindow(l.hwnd, &topLabelViewer);
	topLabelViewer.label.fontHeight = 25;

	hStatuisBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int parts[]{ 300, 650, 900, 1200 };
	SendMessage(hStatuisBar, SB_SETPARTS, dimention_of(parts), (LPARAM)parts);
	HFONT font = CreateFont(30, 0, 0, 90, 900, TRUE, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial");
	SendMessage(hStatuisBar, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	SendMessage(hStatuisBar, SB_SETMINHEIGHT, 30, (LPARAM)0);

	AScanKeyHandler::Stop();

	VL::foreach<viewers_list, Common::__create_window__>()(viewers, l.hwnd);

	UpdateOptions();
	return 0;
}

void AScanWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	AppKeyHandler::Stop();
	delete this;
}

struct __move_window_data__
{
	AScanWindow *scanWindow;
	int y, width, height, maxYHeight, lengthTube;
};

template<class O, class P>struct __move_window__
{
	void operator()(O &o, P &p)
	{
		unsigned count = ALLPatramsItems<LanParametersTable>().get<PacketSize>().value;
		o.tchart.maxAxesX = count;
		o.tchart.count = count;
		o.line.count = count;
		o.gainLine.count = count;
		if (p.scanWindow->XinMM)
		{
			o.tchart.maxAxesX *= ALLPatramsItems<TresholdsTable>().get<SoundSpeed>().value;
			o.tchart.maxAxesX /= 2000.0 * ALLPatramsItems<LanParametersTable>().get<Frequency>().value;
		}
		TSize size{ o.hWnd, WM_SIZE, 0, (WORD)p.width, (WORD)p.height };
		SendMessage(MESSAGE(size));
		MoveWindow(o.hWnd, 0, p.y, p.width, p.height, TRUE);
		p.y += p.height;
	}
};

void AScanWindow::operator()(TSize &l)
{
	if (SIZE_MINIMIZED == l.resizing || 0 == l.Width || 0 == l.Height) return;
	toolBar.Size();
	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	MoveWindow(hStatuisBar, 0, 0, 0, 0, FALSE);
	RECT rs;
	GetClientRect(hStatuisBar, &rs);
	static const int topLabelHeight = 40;
	int y = rt.bottom - 1;
	MoveWindow(topLabelViewer.hWnd, 0, y, l.Width, topLabelHeight, true);

	int t = (l.Height - rs.bottom - rt.bottom - topLabelHeight) / VL::Length<viewers_list>::value;
	y += topLabelHeight;
	//
	__move_window_data__ data{this, y, l.Width, t, l.Height - rs.bottom, 100 };
	VL::foreach<viewers_list, __move_window__>()(viewers, data);

}

void AScanWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void AScanWindow::operator()(TGetMinMaxInfo &l)
{
	if (NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 400;
		l.pMinMaxInfo->ptMinTrackSize.y = 400;
	}
}

HANDLE handleTimer = INVALID_HANDLE_VALUE;
void AScanWindow::operator()(TClose &l)
{
	if (INVALID_HANDLE_VALUE != handleTimer) 
	{
		DeleteTimerQueueTimer(NULL, handleTimer, NULL);
		handleTimer = INVALID_HANDLE_VALUE;
	}
	destroy = true;
	bool tresh = TestX<TresholdsTable>(computeFrame.treshItems);
	bool flt   = TestX<FiltersTable>(computeFrame.paramFlt);

	if (tresh || flt)
	{
		if (IDYES == MessageBox(l.hwnd, L"Данные были изменены. Сохранить изменения?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
		{
			if (TypesizePasswordDlg().Do(l.hwnd))
			{
				CBase base(ParametersBase().name());
				if (base.IsOpen())
				{
					if (tresh) StoreBaseX<TresholdsTable>(base, computeFrame.treshItems);
					if (flt)StoreBaseX<FiltersTable>(base, computeFrame.paramFlt);
					MessageBox(l.hwnd, L"Данные сохранены!", L"Cообщение", MB_ICONEXCLAMATION | MB_OK);
				}
			}
		}
	}
	Stop();
	DestroyWindow(l.hwnd);
}

void AScanWindow::SetThresh()
{
	SetTresh(computeFrame.treshItems, viewers);
	SwitchBipolar(computeFrame.bipolar);
}

struct __update_sens_data__
{
	AScanWindow &owner;
	unsigned offs;
	__update_sens_data__(AScanWindow &owner, unsigned offs)
		: owner(owner)
		, offs(offs)
	{}
};
template<class O, class P>struct __update_sens__;
template<int N, class P>struct __update_sens__<AScanWindow::Sens<N>, P>
{
	typedef AScanWindow::Sens<N> O;
	void operator()(O &o, P &p)
	{
		unsigned offs = p.offs + N * p.owner.computeFrame.packetSize;
		p.owner.computeFrame.Frame(N, offs, o.data);
		o.line.count = p.owner.computeFrame.packetSize;
		RepaintWindow(o.hWnd);
	}
};

void AScanWindow::operator()(TTimer &l)
{
	unsigned offs = computeFrame.framesCount;
	if (offs > computeFrame.packetSize * 9000) computeFrame.framesCount = 0;
	
	offs -= computeFrame.packetSize * App::count_sensors;
	if (offs < 0) offs = 0;
	__update_sens_data__ data(*this, offs);
	VL::foreach<viewers_list, __update_sens__>()(viewers, data);
}

void AScanWindow::SwitchBipolar(bool b)
{
	VL::foreach<viewers_list, __switch_bipolar__>()(viewers, b);
	computeFrame.bipolar = b;
}

template<class O, class P>struct __update_sens__gate__;
template<int N, class P>struct __update_sens__gate__<AScanWindow::Sens<N>, P>
{
	typedef AScanWindow::Sens<N> O;
	void operator()(O &o, P &p)
	{
		o.gainLine.maxAxeY = 1.1 * p.owner.computeFrame.treshItems.get<Num<AlarmGainStop, N>>().value;
		p.owner.computeFrame.Gain(N, o.gain);
		o.gainLine.count = p.owner.computeFrame.packetSize;
		RepaintWindow(o.hWnd);
	}
};

void AScanWindow::Start()
{
	UpdateOptions();
	idTimer = SetTimer(hWnd, 200, 123, NULL);
	AScanKeyHandler::Run();
	
	Singleton<LanDevice>::Instance().Start();
	Sleep(500);
	device1730.WriteOutput(generatorBit);
}

void AScanWindow::UpdateOptions()
{
	SetParam(computeFrame, computeFrame.treshItems);
	computeFrame.UpdateFiltre();

	//график корректировки усиления канала
	__update_sens_data__ data(*this, 0);
	VL::foreach<viewers_list, __update_sens__gate__>()(viewers, data);
}

VOID CALLBACK AScanWindow_WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	if (INVALID_HANDLE_VALUE == handleTimer)return;
	AScanKeyHandler::Stop();
	DeleteTimerQueueTimer(NULL, handleTimer, NULL);
	handleTimer = INVALID_HANDLE_VALUE;
}

void AScanWindow::Stop()
{
	KillTimer(hWnd, idTimer);
	AScanKeyHandler::Disable();

	device1730.WriteOutput(0, generatorBit);
	Singleton<LanDevice>::Instance().Stop();
	if(!destroy)CreateTimerQueueTimer(&handleTimer, NULL, AScanWindow_WaitOrTimerCallback, NULL, 2000, 0, WT_EXECUTEONLYONCE);
}
