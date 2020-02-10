#pragma once
#include "window_tool\WindowsEventTemplate.hpp"
#include "DlgTemplates\ParamDlg.h"
#include "window_tool\GroupBox.h"

namespace Dialog
{
	template<class List>struct __get_height_tmpl__;
	template<class Head, class ...Tail>struct __get_height_tmpl__<Vlst<Head, Tail...> >
	{
		static const int value = Head::DY + __get_height_tmpl__<Vlst<Tail...> >::value;
	};
	template<>struct __get_height_tmpl__<Vlst<>>
	{
		static const int value = 0;
	};
	template<class T, class Owner>struct DlgItem2;
	template<class T>struct GroupBox
	{
		typedef typename T::Result Result;
		static const int DY = __get_height_tmpl__<VL::TypeToTypeLstParam1<Result, DlgItem2, NullType>::Result>::value;
	};

#define GROUP_BOX(...) Dialog::GroupBox<TL::MkTlst<__VA_ARGS__>>
	
	template<class List>struct __del_group_box__;
	template<class Head, class ...Tail>struct __del_group_box__<Vlst<Head, Tail...>>
	{
		typedef typename VL::Append<Head, typename __del_group_box__<Vlst<Tail...>>::Result>::Result Result;
	};
	template<class List, class ...Tail>struct __del_group_box__<Vlst<GroupBox<List>, Tail...> >
	{
		typedef typename VL::MultyList<Vlst<
			typename List::Result
			, typename __del_group_box__<Tail...>::Result
		> >::Result Result;
	};
	template<>struct __del_group_box__<Vlst<> >
	{
	   typedef Vlst<> Result;
	};

	template<class T>struct isParentExist
	{
		typedef typename VL::_if<
			WET::IsFuncExist<T, TInitDialog>::value
			, T
			, typename isParentExist<typename WET::IsParent<T>::Result>::Result
		>:: Result Result;
	};
	template<>struct isParentExist<Vlst<> >
	{
		typedef Vlst<> Result;
	};
	template<class T>struct EventHandler
	{
		template<class O, class P>struct loc
		{
			template<class T, T>struct Helper{};
			template<class X, class Z>static double Is(X *, Helper<void(X::*)(Z &), &X::operator()> * = NULL);
			template<class X, class Z>static char Is(...);
			bool operator()(P &p)
			{
				if(WET::TypeToEvent<typename O::PAR>::value != p.mess.uMsg) return true;
				static const bool b = sizeof(double) == sizeof(Is<typename O::OBJ, typename O::PAR>((typename O::OBJ *)0));
				p.result = WET::Wrap<b>()((typename O::PAR &)p.mess, (EventHandler<typename O::OBJ> &)p);
				return false;
			}
		};
		template<class List, class T>struct TestNotNullType
		{
			typedef List Result;
		};
		template<class T>struct TestNotNullType<NullType, T>
		{
			///\brief класс должен иметь обработчик в виде- LRESULT T::operator()(XXX &) или void T::operator()(XXX &);
			///где: XXX - TMouseMove, TSize, TPaint, TActivate(добавить при необходимости в список type_events_all_list, см. выше)
			typedef typename T::_class_does_not_have_any_handler nonexist;
		};
		typedef typename TestNotNullType<typename WET::AddTypeEvent<WET::type_events_all_list, T>::Result, T>::Result list_0;
		TMessage &mess;
		T &obj;
		LRESULT result;
	public:
		typedef typename WET::ReTypeDelete<list_0>::Result list;
		EventHandler(TMessage &mess, T &obj)
			: mess(mess)
			, obj(obj)
			, result(0)
		{}
		LRESULT operator()()
		{			
			VL::find<list, loc>()(*this);
			return result;
		}
	};
	template<class T>struct Proc
	{
		template<class Z>struct Init
		{
			LRESULT operator()(TInitDialog &l)
			{
				T *o = (T *)l.lParam;
				SetWindowLongPtr(l.hwnd, GWLP_USERDATA, (LONG_PTR)o);
				(*(Z *)o)(l);
				return TRUE;
			}
		};		
	public:
		static LRESULT CALLBACK Do(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			T *o = (T *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			TMessage m = {hWnd, message, wParam, lParam};
			if(NULL != o)
			{
				return EventHandler<T>(m, *o)();
			}	
			else if(message == WM_INITDIALOG)
			{
				Init<isParentExist<T>::Result>()((TInitDialog &)m);
				return TRUE;
			}
			return FALSE;
		}	
	};

	template<class TemplDialog>struct __command_data__
	{
		HWND hwnd;
		WPARAM id;
		TemplDialog &owner;
		__command_data__(HWND hwnd, WPARAM id, TemplDialog &owner)
			: hwnd(hwnd)
			, id(id)
			, owner(owner)
		{}
	};

	template<class List>struct __btn_width__;
	template<class Head, class ...Tail>struct __btn_width__<Vlst<Head, Tail...> >
	{
		static const int value = Head::width + __btn_width__<Vlst<Tail...> >::value;
	};
	template<>struct __btn_width__<Vlst<>>
	{
		static const int value = 0;
	};
	struct __make_btn_data__
	{
		int offs;
		int height;
		HWND h;
		__make_btn_data__(int offs, int height, HWND h)
			: offs(offs)
			, height(height)
			, h(h)
		{}
	};	

	template<class T, class Owner>struct DlgItem2
	{
		static const int DY = DlgSubItems<T, typename T::type_value>::DY;
		Owner &owner;
		T &value;
		HWND hWnd;
		DlgItem2(Owner &o) : owner(o), value(o.table.items.get<T>()){}
		void Init(HWND h, int &x, int &width, int &dy)
		{
			hWnd = DlgSubItems<T, typename T::type_value>().Init(h, x, width, dy, value);
		}
	};

	template<class T>struct DlgItem2<T, NullType>
	{
		static const int DY = DlgSubItems<T, typename T::type_value>::DY;
		T value;
		HWND hWnd;
		void Init(HWND h, int &x, int &width, int &dy)
		{
			hWnd = DlgSubItems<T, typename T::type_value>().Init(h, x, width, dy, value);
		}
	};

	template<class X, class Owner>struct DlgItem2<GroupBox<X>, Owner>
	{
		Owner *owner;
		HWND hWnd;
		DlgItem2(Owner *o) : owner(o){}		
	};

	template<class X>struct DlgItem2<GroupBox<X>, NullType>
	{
		HWND hWnd;
	};

	template<class X, class Owner, class P>struct __ok_btn__<DlgItem2<GroupBox<X>,Owner>, P>
	{
		template<class Z>void operator()(Z *, P *){}
	};

	template<class X, class Owner, class P>struct __test__<DlgItem2<GroupBox<X>,Owner>, P>
	{
		template<class O>bool operator()(O *o, P *p)
		{
			return true;
		}
	};

	template<class T>struct __table_data__X
	{
		HWND h; 
		int &x;
		int &width; 
		int &height;
		T &items;
		__table_data__X(HWND h, int &x, int &width, int &height, T &t): h(h), x(x), width(width), height(height), items(t){}
	};

	template<class O, class P>struct __init__X
	{
		void operator()(P &p)
		{
			p.items.get<O>().Init(p.h, p.x, p.width, p.height);	
		}
	};

	template<class O, class P>struct __init_unit__
	{
		void operator()(P &p)
		{
			p.items.get<O>().Init(p.h, p.x, p.width, p.height);
		};
	};

	template<class X, class Owner, class P>struct __init__X<DlgItem2<GroupBox<X>, Owner>, P>
	{
		typedef DlgItem2<GroupBox<X>, Owner> O; 
		
		void operator()(P &p)
		{
			typedef typename VL::TypeToTypeLstParam1<typename GroupBox<X>::Result, DlgItem2, Owner>::Result list; 
			int height = 22 + GroupBox<X>::DY;
			
			HWND hWnd = GroupBoxWindow(ParamTitle<GroupBox<X>>()(),  p.x, p.height, p.width - 15, height, p.h);
     		p.height += height;
			HWND storeHWND = p.h;
			int storeHeight = p.height;
			p.h = hWnd;
			p.height = 17;
			int store_x = p.x;
			p.x = 5;
			VL::for_each<list, __init_unit__>()(p);
			p.x = store_x;
			p.h = storeHWND;
			p.height = storeHeight + 5;
		}
	};
	
#pragma warning(disable: 4355)
	template<class BaseParam, class TableParam
		, class List = typename TableParam::items_list
		, int widthP = 550
		, class ButtonsList = Vlst<OkBtn, CancelBtn>
		, template<class, class>class Wrap = DlgItem2>struct Templ
	{
		typedef typename VL::TypeToTypeLstParam1<List, Wrap, Templ>::Result original_list;
		typedef BaseParam Base;
		typedef TableParam Table;
		Table &table;	
		typedef typename VL::TypeToTypeLstParam1<typename __del_group_box__<List>::Result, Wrap, Templ>::Result list;
		VL::Factory<list> items;
		VL::Factory<ButtonsList> buttons;
		void operator()(TInitDialog &e)
		{
			int width = widthP;
			int height = 10;
			int xOffs = 5;

			__table_data__X<VL::Factory<list>>param(e.hwnd, xOffs, width, height, items);
			VL::for_each<original_list, __init__X>()(param);
			
			int offs = __btn_width__<ButtonsList>::value + (VL::Length<ButtonsList>::value - 1) * 10;
			
			offs = (width - offs) / 2;
			height += 10;
			
			__make_btn_data__ make_btn_data(offs, height, e.hwnd);
			VL::for_each<ButtonsList, __make_btn__>()(buttons, make_btn_data);
			
			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);
			
			height += 75;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(e.hwnd, x, y, width, height, FALSE);
		}
		LRESULT operator()(TCommand &e)
		{
			__command_data__<Templ> command_data(e.hwnd, e.id, *this);
			return !VL::find<ButtonsList, __command__>()(buttons, command_data);
		}

		Templ(Table &table_) : table(table_), items(*this){}
		bool Do(HWND hWnd, wchar_t *title)
		{
			return TemplDlg_Do(hWnd, title, (DLGPROC)Proc<Templ>::Do, (LPARAM)this);
		}
	};
}
/*	   Пример применения 
#include "stdafx.h"
#include "GroupBox.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"
#include "ParamDlgNew.h"

MIN_EQUAL_VALUE(DeadAreaMM0<Cross>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Cross>, 500)

MIN_EQUAL_VALUE(DeadAreaMM1<Cross>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Cross>, 500)

MIN_EQUAL_VALUE(DeadAreaMM0<Long>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Long>, 500)
							
MIN_EQUAL_VALUE(DeadAreaMM1<Long>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Long>, 500)

template<class T>struct ParamTitle<DeadAreaMM0<T>>{wchar_t *operator()(){return L"начало трубы";}};
template<class T>struct ParamTitle<DeadAreaMM1<T>>{wchar_t *operator()(){return L"конец трубы";}};

static const wchar_t *turnOnControlCircuit = L"включите цепи управления";
PARAM_TITLE(GROUP_BOX(DeadAreaMM0<Cross>, DeadAreaMM1<Cross>), L"Поперечный контроль")
PARAM_TITLE(GROUP_BOX(DeadAreaMM0<Long>, DeadAreaMM1<Long>), L"Продольный контроль")

DEFINE_PARAM(Mess, bool, false)

template<class Owner>struct Dialog::DlgItem2<Mess, Owner>
{
	static const int DY = 30;
	Owner *owner;
	HWND hWnd;
	HFONT hFont;
	DlgItem2(Owner *o) : owner(o)
	{
		hFont = CreateFont (26, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
				DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"));
	}
	~DlgItem2()
	{
			DeleteObject(hFont);
	}
	void Init(HWND h, int &x, int &width, int &dy)
	{
		hWnd = CreateWindow(L"static", turnOnControlCircuit
			, WS_VISIBLE | WS_CHILD
			, x, dy, 600, 30, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;
		SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
	}
};
template<class Z, class P>struct __ok_btn__< Dialog::DlgItem2<Mess,Z>, P>
{
	void operator()( Dialog::DlgItem2<Mess,Z> *, P *){}
};

DEFINE_PARAM(GroupBox, bool, false)


template<class O, class P>struct __height__
{
	void operator()(P &p)
	{
		p += O::DY;
	}
};

typedef Dialog::Templ<ParametersBase, DeadAreaTable	
		, TL::MkTlst<
		Mess
		, GROUP_BOX(DeadAreaMM0<Cross>, DeadAreaMM1<Cross>)
		, GROUP_BOX(DeadAreaMM0<Long>, DeadAreaMM1<Long>)
		>::Result
		> TypeColorDlg;

struct ColorDlg: TypeColorDlg
{
	typedef TypeColorDlg Parent;
	template<class O, class P>struct __color_static__
	{
		unsigned operator()(O &, P &){return 0;}
	};
	template<class Z, class P>struct __color_static__<Dialog::DlgItem2<Mess, Z>, P>
	{
		typedef Dialog::DlgItem2<Mess, Z> O;
		unsigned operator()(O &o, P &p)
		{
			if(o.hWnd == p.hCondtol)
			{
				SetTextColor(p.hdc, RGB(0,0,255));
				SetBkColor(p.hdc, RGB(250,250,0));
				return (LRESULT)CreateSolidBrush(RGB(250,250,0));
			}
			return FALSE;
		}
	};
	LRESULT operator()(TCtlColorStatic &l)
	{
		return TL::find_ret<list, __color_static__>()(items, l);
	}
	ColorDlg(DeadAreaTable &t): TypeColorDlg(t){}
};

void DeadZonesDlg(HWND h)
{
	
	if(ColorDlg(Singleton<DeadAreaTable>::Instance()).Do<ColorDlg>(h, L"Мёртвые зоны"))
	{
	}

}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	DeadZonesDlg(NULL);
	return 0;
}
*/
