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

LRESULT AScanWindow::operator()(TCreate &l)
{
	VL::CopyFromTo(Singleton< TresholdsTable>::Instance().items, treshItems);
	SetThresh();
	aScanAuto.SetHandle(this, &AScanWindow::Update);


	AppKeyHandler::DisableAll();
	Menu<AScanWindowMenu::Menu>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	CreateChildWindow(l.hwnd, &topLabelViewer);
	topLabelViewer.label = (wchar_t *)L"<ff00>Test";
	topLabelViewer.label.fontHeight = 25;

	hStatuisBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int parts[]{ 300, 650, 900, 1200 };
	SendMessage(hStatuisBar, SB_SETPARTS, dimention_of(parts), (LPARAM)parts);
	HFONT font = CreateFont(30, 0, 0, 90, 900, TRUE, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial");
	SendMessage(hStatuisBar, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	SendMessage(hStatuisBar, SB_SETMINHEIGHT, 30, (LPARAM)0);

	AScanKeyHandler::Stop();

	VL::foreach<viewers_list, Common::__create_window__>()(viewers, l.hwnd);
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
	int y, width, height, maxYHeight, lengthTube;
};

template<class O, class P>struct __move_window__
{
	void operator()(O &o, P &p)
	{
		o.tchart.maxAxesX = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
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
	__move_window_data__ data{ y, l.Width, t, l.Height - rs.bottom, 100 };
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

template<class O, class P>struct __compare_tresh__
{
	bool operator()(O &o, P &p)
	{
		return o.value == p.get<O>().value;
	}
};

template<class T>void StoreBaseX(CBase &base, typename T::TItems &from)
{
	auto &to = Singleton<T>::Instance();
	VL::CopyFromTo(from, to.items);
	int id = CurrentId<ID<T> >();
	if (1 == CountId<ID<T> >(base, id))
	{
		UpdateWhere<T>(to, base).ID(id).Execute();
	}
	else
	{
		Insert_Into<T>(to, base).Execute();
	}
}

template<class T>bool TestX(typename T::TItems &x)
{
	return VL::find<T::items_list, __compare_tresh__>()(Singleton<T>::Instance().items, x);
}

void AScanWindow::operator()(TClose &l)
{
	aScanAuto.Stop();
	bool tresh = TestX<TresholdsTable>(treshItems);
	bool flt   = TestX<FiltersTable>(computeFrame.paramFlt);

	if (tresh && flt)
	{
		DestroyWindow(l.hwnd);
	}
	else
	{
		if (IDYES == MessageBox(l.hwnd, L"Данные были изменены. Сохранить изменения?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
		{
			if (TypesizePasswordDlg().Do(l.hwnd))
			{
				CBase base(ParametersBase().name());
				if (base.IsOpen())
				{
					if (!tresh) StoreBaseX<TresholdsTable>(base, treshItems);
					if (!flt)StoreBaseX<FiltersTable>(base, computeFrame.paramFlt);
					MessageBox(l.hwnd, L"Данные сохранены!", L"Cообщение", MB_ICONEXCLAMATION | MB_OK);
				}
			}
		}
		DestroyWindow(l.hwnd);
	}
}

struct __set_thresh_data__
{
	unsigned alThr_color;
	double   alThr_value;
	int      alThr_startOffs;
	int      alThr_stopOffs;
	unsigned btmRefThr_color;
	double   btmRefThr_value;
	int      btmRefThr_startOffs;
	int      btmRefThr_stopOffs;
};

template<class O, class P>struct __set_thresh__
{
	void operator()(O &o, P &p)
	{
		auto &alThr = o.tchart.items.get<AScanViewer::AlThr>();
		auto &btmRefThr = o.tchart.items.get<AScanViewer::BtmRefThr>();

		alThr.color = p.alThr_color;
		alThr.value = p.alThr_value;
		alThr.startOffs = p.alThr_startOffs;
		alThr.stopOffs = p.alThr_stopOffs;
		btmRefThr.color = p.btmRefThr_color;
		btmRefThr.value = p.btmRefThr_value;
		btmRefThr.startOffs = p.btmRefThr_startOffs;
		btmRefThr.stopOffs = p.btmRefThr_stopOffs;
	}
};

void AScanWindow::SetThresh()
{
	auto &color = Singleton<ColorTable>::Instance().items;

	__set_thresh_data__ data = {
	   color.get< Clr<defect>>().value
	   , treshItems.get<AlarmThresh>().value
	   , treshItems.get<AlarmThreshStart>().value
	   , treshItems.get<AlarmThreshStop>().value
	   , color.get< Clr<noBottomReflection>>().value
	   , treshItems.get<BottomReflectionThresh>().value
	   , treshItems.get<BottomReflectionThreshStart>().value
	   , treshItems.get<BottomReflectionThreshStop>().value
	};
	VL::foreach<viewers_list, __set_thresh__>()(viewers, data);
}

void AScanWindow::SetBipolar()
{
	computeFrame.bipolar = TestMenu<MenuItem<AScanWindowMenu::BiPolar>>(hWnd);
}

struct __update_sens_data__
{
	AScanWindow &owner;
	int offs;
	__update_sens_data__(AScanWindow &owner, int offs)
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
		auto &w = p.owner.viewers.get<O>();
		p.owner.computeFrame.Frame(N, p.offs, w.data);
		w.line.count = p.owner.computeFrame.packetSize;
		RepaintWindow(w.hWnd);
	}
};

void AScanWindow::Update()
{
	int offs = computeFrame.framesCount;
	offs /= computeFrame.packetSize;
	offs /= App::count_sensors;
	offs -= 10;
	if (offs < 0) offs = 0;

	__update_sens_data__ data(*this, offs);
	VL::foreach<viewers_list, __update_sens__>()(viewers, data);
}

template<class A, class B>void __assignment_param__(A &a, B &b)
{
	a.value =     b.value;
	a.startOffs = b.startOffs;
	a.stopOffs =  b.stopOffs;
	a.color =     b.color;
	a.widthPen =  b.widthPen;
	a.dashStyle = b.dashStyle;
}

template<class O, class P>struct __switch_bipolar__
{
	void operator()(O &o, P &p)
	{
		auto &c = o.tchart.items;
		auto &chart = o.tchart;
		if (p)
		{
			__assignment_param__(c.get<AScanViewer::NAlThr>(), c.get<AScanViewer::AlThr>());
			c.get<AScanViewer::NAlThr>().value *= -1;
			__assignment_param__(c.get<AScanViewer::NBtmRefThr>(), c.get<AScanViewer::BtmRefThr>());
			c.get<AScanViewer::NBtmRefThr>().value *= -1;
			chart.minAxesY = -100;
		}
		else
		{
			c.get<AScanViewer::NAlThr>().value = 0;
			c.get<AScanViewer::NBtmRefThr>().value = 0;
			chart.minAxesY = 0;
		}
	}
};

void AScanWindow::SwitchBipolar(bool b)
{
	VL::foreach<viewers_list, __switch_bipolar__>()(viewers, b);
}

void AScanWindow::Start()
{
	SetBipolar();
	aScanAuto.Start();
}

void AScanWindow::Stop()
{
	aScanAuto.Stop();
}
