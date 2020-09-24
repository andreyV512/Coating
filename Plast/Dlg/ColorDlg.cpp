#include "Dlg.h"
#include "DlgTemplates\ParamDlgNew.h"
#include "templates/typelist.hpp"
#include <GdiPlus.h>
#include "window_tool/DlgFiles.h"
#include "MessageText/status.h"
#include "window_tool/Emptywindow.h"
#include "Windows/MainWindow/MainWindow.h"

namespace {
	struct DefaultBtn;
	typedef Dialog::Templ<ParametersBase, ColorTable
		, ColorTable::items_list
		, 550
		, Vlst<OkBtn, CancelBtn, DefaultBtn>
	> ClrDlg;
	void __color_wm_paint(HWND hwnd, int color)
	{
		PAINTSTRUCT p;
		HDC hdc = BeginPaint(hwnd, &p);
		{
			Gdiplus::Graphics g(hdc);
			RECT r;
			GetClientRect(hwnd, &r);
			g.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(color)), 0, 0, r.right, r.bottom);
		}
		EndPaint(hwnd, &p);
	}

	int __color_l_button_down(HWND hwnd)
	{
		ColorDialog c(hwnd);
		char d[4] = { -1 };
		if (c())
		{
			*(int *)d = c.color.rgbResult | 0xff000000;
			char t = d[0];
			d[0] = d[2];
			d[2] = t;
			InvalidateRect(hwnd, NULL, true);
		}
		return *(int *)d;
	}

	template<class T>struct __color__
	{
		static LRESULT CALLBACK Proc_(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_PAINT:
			{
				T *t = (T *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				if (t)
				{
					__color_wm_paint(hwnd, t->value);
				}
			}
			return 0;
			case WM_LBUTTONDBLCLK:
			case WM_LBUTTONDOWN:
			{
				T *t = (T *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				if (t)
				{
					t->value = __color_l_button_down(hwnd);
				}
			}
			return 0;
			}
			return CallWindowProc(OldWndProc, hwnd, msg, wParam, lParam);
		}
		static WNDPROC OldWndProc;
	};

	template<class T>WNDPROC __color__<T>::OldWndProc;

	template<class T>struct ColorButtonItem
	{
		static const int DY = 25;
		HWND Init(HWND h, int &x, int &width, int &dy, T &t)
		{
			HWND hWnd = CreateWindow(L"button", L""
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				, x, dy, 100, 30, h, (HMENU)0, (HINSTANCE)::GetModuleHandle(NULL), 0
			);
			__color__<T>::OldWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)__color__<T>::Proc_);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)&t);
			CreateWindow(L"static", ParamTitle<T>()()
				, WS_VISIBLE | WS_CHILD
				, 100 + 20, dy + 7, dlg_width, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
			dy += 35;

			return hWnd;
		}
	};

#define DLG_SUB(n, txt)template<>struct DlgSubItems<n, unsigned>: ColorButtonItem<n>{};\
template<>struct __data_from_widget__<Dialog::DlgItem2<n, ClrDlg>, unsigned>\
{\
    int operator()(Dialog::DlgItem2<n, ClrDlg> &o)\
	{\
		return o.value.value;\
	}\
};\
DO_NOT_CHECK(n)\
PARAM_TITLE(n, txt)

#define DLG_SUB_LIST(n, txt, ...)template<>struct DlgSubItems<n<__VA_ARGS__>, unsigned>: ColorButtonItem<n<__VA_ARGS__>>{};\
template<>struct __data_from_widget__<Dialog::DlgItem2<n<__VA_ARGS__>, ClrDlg>, unsigned>\
{\
    int operator()(Dialog::DlgItem2<n<__VA_ARGS__>, ClrDlg> &o)\
	{\
		return o.value.value;\
	}\
};\
DO_NOT_CHECK(n<__VA_ARGS__>)\
PARAM_TITLE(n<__VA_ARGS__>, txt)

	DLG_SUB(Clr<Norm>, L"Норма")
		DLG_SUB(Clr<deadZone>, L"Мёртвая зона")
		DLG_SUB(Clr<SensorOff >, L"Датчик отключен")
		DLG_SUB(Clr<defect   >, L"Дефект")
		DLG_SUB(Clr<noBottomReflection>, L"Нет донного отражения")
		DLG_SUB_LIST(Clr, L"Брак, нет донного отражения", noBottomReflection, defect)

	struct DefaultBtn
	{
		static const int width = 160;
		static const int height = 30;
		static const int ID = IDRETRY;
		wchar_t *Title() { return (wchar_t *)L"Встроенные настройки"; }
		template<class Owner>void BtnHandler(Owner &o, HWND h)
		{
			if (TestPassword<Owner::Base, Owner::Table>()(h))
			{
				CBase base(Owner::Base().name());
				if (base.IsOpen())
				{
					ColorTable color;
					UpdateWhere<ColorTable>(color, base).ID(__id__).Execute();
					CopyFromTo(color.items, Singleton<ColorTable>::Instance().items);
				}
				EndDialog(h, FALSE);
			}
		}
	};
}
void ColorDlg::Do(HWND h)
{
	ColorTable color;
	CopyFromTo(Singleton<ColorTable>::Instance().items, color.items);
	if (ClrDlg(color).Do(h, (wchar_t *)L"Цвета"))
	{
		CopyFromTo(color.items, Singleton<ColorTable>::Instance().items);
		StatusData::UpdateColor();
		RepaintWindow<MainWindow>();
	}
}

