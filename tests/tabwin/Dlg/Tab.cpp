#include "Tab.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "window_tool/EmptyWindow.h"
#include "Log/LogMessages.h"
#include "tools_debug/DebugMess.h"
#include "window_tool/TEvent.h"
#include "window_tool/MenuAPI.h"

typedef VL::CreateNumList<VL::IntToType, 0, LogMess::MAX_MESS_ID - 1>::Result num_list;

template<class T>struct ms
{
	typedef bool type_value;
	bool value;
	static const wchar_t *name() { return T::mess(); }
};

template<class List>struct __mk_list__;
template<class Head, class ...Tail>struct __mk_list__<Vlst<Head, Tail...>>
{
	typedef typename VL::Append<
		ms<typename IDtoMess<Head::value>::Result>
		, typename __mk_list__<Vlst<Tail...>>::Result
	>::Result Result;
};
template<>struct __mk_list__<Vlst<>>
{
	typedef Vlst<> Result;
};

template<class T>struct ParamTitle<ms<T>>{wchar_t *operator()(){return (wchar_t *)ms<T>::name();}};

struct xxTable
{
	typedef __mk_list__<num_list>::Result items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
};

template<class O, class P>struct Print
{
	void operator()(P &p)
	{
		const wchar_t *w = O::name(); 

		dprint("%d %S\n", ++p, w);
	//	dprint("%S\n", O::name());			  

	}
};

class xxOkBtn: public TEvent
{
public:
	static const int width = 120;
	static const int height = 30;
	wchar_t *Title() { return (wchar_t *)L"Применить"; }
	void Do(TCommand &l)
	{
		EndDialog(l.hwnd, TRUE);
	}
};
class xxCancelBtn : public TEvent
{
public:
	static const int width = 120;
	static const int height = 30;
	wchar_t *Title() { return (wchar_t *)L"Закрыть"; }
	void Do(TCommand &l)
	{
		EndDialog(l.hwnd , FALSE);
	}
};

template<class O, class P>struct __make_btn_xx__
{
	void operator()(O &o, P &p)
	{
		HWND hWnd = CreateWindow(L"button", o.Title()
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			, p.offs, p.height, O::width, 30, p.h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);
		p.offs += O::width + 10;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)&o);
	}
};

template<class List, int start, int stop>struct ShowItems
{
	template<class O, class P>struct loc
	{
		void operator()(O &o)
		{
			static const int i = VL::IndexOf<List, O>::value;
			ShowWindow(o.hWnd, i >= start && i < stop ? SW_SHOW : SW_HIDE);
		}
	};
	template<class Items>void operator()(Items &items)
	{
		VL::foreach<List, loc>()(items);
	}
};

template<int ItemsOnPage, class List>struct ShowPage
{
	static const int count = VL::Length<List>::value;
	static const int count_pages = count / ItemsOnPage + (0 != count % ItemsOnPage);
	template<class Items>struct __data__
	{
		Items *items;
		int page;
	};
	template<class O, class P>struct __page__
	{
		bool operator()(P &p)
		{
			if (O::value == p.page)
			{
				ShowItems<List, O::value * ItemsOnPage, (1 + O::value) * ItemsOnPage>()(*p.items);
				return false;
			}
			return true;
		}
	};
	template<class Items>void operator()(Items &items, int page)
	{
		__data__<Items> data{ &items, page };
		VL::find<typename VL::CreateNumList<VL::IntToType, 0, count_pages - 1>::Result, __page__>()(data);
	}
};

template<class T, class Owner>class xxDlgItem : public TEvent
{
public:
	static const int DY = DlgSubItems<T, typename T::type_value>::DY;
	Owner &owner;
	T &value;
	HWND hWnd;
	xxDlgItem(Owner &o) : owner(o), value(o.table.items.get<T>()) {}
	void Init(HWND h, int &x, int &width, int &dy)
	{
		hWnd = DlgSubItems<T, typename T::type_value>().Init(h, x, width, dy, value);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
		Button_SetCheck(hWnd, value.value ? BST_CHECKED : BST_UNCHECKED);
	}
	void Do(TCommand &l)
	{
		value.value = BST_CHECKED == Button_GetCheck(hWnd);
	}
};

template<
	int ItemsOnPage
	, class TableParam
	, class ButtonsList
	, int widthP = 350
>
	struct xxTempl: Dialog::Templ<
	Vlst<>
	, TableParam
	, typename TableParam::items_list
	, widthP
	, ButtonsList
	, Vlst<>
	, xxDlgItem
	>
{
	typedef Dialog::Templ<
		Vlst<>
		, TableParam
		, typename TableParam::items_list
		, widthP
		, ButtonsList
		, Vlst<>
		, xxDlgItem
	> Self;
	struct __create_pages_data__
	{
		HWND h;
		int counter, x, y, width, height, maxHeight;
	};
	template<class O, class P>struct __create_pages__
	{
		void operator()(O &o, P &p)
		{
			++p.counter;
			o.Init(p.h, p.x, p.width, p.height);
			if (p.counter >= ItemsOnPage)
			{
				if (p.maxHeight < p.height)	p.maxHeight = p.height;
				p.height = p.y;
				p.counter = 0;
			}
		}
	};

	HWND hwndTab;

	void operator()(TInitDialog &e)
	{
		int width = widthP;
		int height = 35;
		int xOffs = 5;
		RECT rcClient;
		GetClientRect(e.hwnd, &rcClient);
		hwndTab = CreateWindow(WC_TABCONTROL, L"",
			WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
			0, 0, rcClient.right, rcClient.bottom,
			e.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL);
		wchar_t txt[128]{};
		TCITEM tie{};
		tie.mask =  TCIF_TEXT | TCIF_IMAGE;
		tie.iImage = -1;
		tie.pszText = txt;

		typedef ShowPage<ItemsOnPage, Self::original_list> show_page;

		for (int i = 0; i < show_page::count_pages; ++i)
		{
			_itow(1 + i, txt, 10);
			if (-1 == TabCtrl_InsertItem(hwndTab, i, &tie) )
			{
				dprint("ERROR %d\n", i)	;
			}
		}

		__create_pages_data__ pages_data{e.hwnd, 0, xOffs, height, width - 30, height, 0};
		VL::foreach<Self::original_list, __create_pages__>()(this->items, pages_data);
		show_page()(this->items, 0);

		height = pages_data.maxHeight;

		typedef typename Dialog::__only_buttons__<ButtonsList>::Result __button_list__;
		int offs = Dialog::__btn_width__<__button_list__>::value + (VL::Length<__button_list__>::value - 1) * 10;

		offs = (width - offs) / 2;
		height += 10;

		Dialog::__make_btn_data__ data(offs, height, e.hwnd);
		VL::foreach<__button_list__, __make_btn_xx__>()(this->buttons, data);

		RECT r;
		GetWindowRect(GetDesktopWindow(), &r);
		//
		height += 75;
		int x = r.left + (r.right - r.left - width) / 2;
		int y = r.top + (r.bottom - r.top - height) / 2;
		MoveWindow(e.hwnd, x, y, width, height, FALSE);
	}

	LRESULT operator()(TSize &e)
	{
		if (!SetWindowPos(hwndTab, HWND_TOP, 0, 0, e.Width, e.Height, SWP_SHOWWINDOW))
			return E_FAIL;

		return S_OK;
	}

	LRESULT operator()(TNotify &e)
	{
		switch (e.pnmh->code)
		{
		case BN_CLICKED:
			dprint("BN_CLICKED \n");
			break;
		case TCN_SELCHANGING:
		{
			// Return FALSE to allow the selection to change.
			return FALSE;
		}

		case TCN_SELCHANGE:
		{
			int iPage = TabCtrl_GetCurSel(e.pnmh->hwndFrom);
			ShowPage<ItemsOnPage, Self::original_list>()(this->items, iPage);
			dprint("TCN_SELCHANGE %d\n", iPage);
			break;
		}
		}
		return TRUE;
	}

	LRESULT operator()(TCommand &e)
	{
		EventDo(e);
		return TRUE;
	}

	xxTempl(TableParam &table_) : Self(table_, NULL)
	{}
	bool Do(HWND hWnd, wchar_t *title)
	{
		return TemplDlg_Do(hWnd, title, (DLGPROC)Dialog::Proc<xxTempl>::Do, (LPARAM)this);
	}
};

void Tab::Do(HWND h)
{
	xxTable t;
	if (xxTempl<
		15
		, xxTable
		, Vlst<xxOkBtn, xxCancelBtn>
	>(t).Do(h, (wchar_t *)L"Сообщения"))
	{
		dprint("OK BTN\n");
	}
}
