#pragma once
#include <windows.h>
#include "window_tool/TEvent.h"
#include "templates/typelist.hpp"
#include "tools_debug/DebugMess.h"
//------------------------------------------------------------------------------------------
template<class T>struct MenuListItem{};
template<class T>struct MenuItem{};
template<class T>struct TopMenu{};
template<class T>struct SubMenu{typedef NullType list;};
template<int>struct Separator{};

#pragma warning(disable:4995)
template<class T>struct NameMenu
{
	wchar_t *operator()(HWND)
	{
		static const int i = dimention_of(__FUNCTION__);
		static wchar_t b[i];
		wsprintf(b, L"%S", __FUNCTION__);              
        b[i - 14] = 0;
		return b;
	}
};
template<class T>struct EnableMenuInit
{
	int operator()(HWND)
	{
		return MFS_ENABLED;
	}
};
template<class T>struct Event
#if 1
{
	static void Do(HWND)
	{
		zprint("\n");
	}
}
#endif
;
template<class T>struct ReturnItemMenu
{
	bool operator()(){return true;}
};
//------------------------------------------------------------------------------------------
struct Param
{
	MENUITEMINFO m;
	HMENU h;
	HWND  hWnd;
	int& Index() { static int x; return x; }
	Param(HMENU x, HWND hWnd) : h(x), hWnd(hWnd)
	{
		m.cbSize = sizeof(MENUITEMINFO);
		m.fType = MFT_STRING;
		m.hSubMenu = NULL;
		m.cch = 0;
	}
};
template<class O, class P>struct __insert_menu__;
template<class List>class Menu
{
public:
	HMENU Init(HWND hWnd)
	{
		if(NULL != hWnd) DestroyMenu(GetMenu(hWnd));
		Param param(CreateMenu(), hWnd);
		VL::find<List, __insert_menu__>()(param);
		SetMenu(hWnd, param.h); 
		DrawMenuBar(hWnd);
		return param.h;
		return 0;
	}
};
template<class List>class PopupMenu
{
public:
	template<class P>static void Do(HWND hWnd, P *data)
	{		
		Param param(CreatePopupMenu(), hWnd);
		VL::find<List, __insert_menu__>()(param);
		HMENU hMenu = param.h;
		POINT p;
		GetCursorPos(&p);
		UINT flags = TPM_BOTTOMALIGN | TPM_NONOTIFY | TPM_RETURNCMD;

		flags |= (GetSystemMetrics(SM_MENUDROPALIGNMENT) == 0 ) ? TPM_LEFTALIGN: TPM_RIGHTALIGN;

		if(unsigned d = TrackPopupMenuEx(hMenu, flags, p.x, p.y, hWnd, NULL))
		{	
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_DATA;
			if(GetMenuItemInfo(hMenu, d, false, &mii))
			{
				((void (__cdecl *)(P *))(mii.dwItemData))(data);
			}
		}
		DestroyMenu(hMenu);
	}	
};

template<class O, class P>struct __insert_item_menu__
{
	bool operator()(P &p)
	{
         debug.print(__FUNCTION__);
		 return true;
	}
};
template<class>struct MenuItem;
template<class S, class P>struct __insert_item_menu__<MenuItem<S>, P>
{
	typedef MenuItem<S> O;
	bool operator()(P &p)
	{      
		if(ReturnItemMenu<O>()())
		{
			p.m.fMask = MIIM_DATA | MIIM_TYPE | MIIM_ID | MIIM_STATE;			
			p.m.dwTypeData = NameMenu<O>()(p.hWnd);	
			p.m.dwItemData = (ULONG_PTR)Event<O>::Do;	
		    p.m.wID = p.m.dwItemData & 0xffff;
			p.m.fState = EnableMenuInit<O>()(p.hWnd);
			InsertMenuItemW(p.h, p.Index()++, false, &p.m);
			return true;
		}
		return false;
	}
};

template<class T>struct IsExistList
{
	template<class Z>struct Helper { typedef double Result; };
	template<>struct Helper<Vlst<>> { typedef char Result; };
	template<class Z>static typename Helper<typename Z::list>::Result tst(Z *);
	template<class Z>static char tst(...);
	static const bool  value = sizeof(double) == sizeof(tst<T>(NULL));
};

template<class O, class P, bool b>struct __sub_insert_menu__
{
	template<class Param>void operator()(Param &param, P &p)
	{
		VL::find<typename O::list, __insert_item_menu__>() (param);
		p.m.fMask = MIIM_SUBMENU | MIIM_TYPE | MIIM_DATA | MIIM_ID | MIIM_STATE;
		p.m.hSubMenu = param.h;
	}
};

template<class O, class P>struct __sub_insert_menu__<O, P, false>
{
	template<class Param>void operator()(Param &, P &p)
	{
		p.m.fMask = MIIM_TYPE | MIIM_DATA | MIIM_ID | MIIM_STATE;
		p.m.hSubMenu = NULL;
	}
};

template<class O, class P>struct __insert_menu__
{
	bool operator()(P &p)
	{	
		Param param(CreatePopupMenu(), p.hWnd);
		
		__sub_insert_menu__<O, P, IsExistList<O>::value>()(param, p);

		p.m.dwTypeData = NameMenu<O>()(p.hWnd);	
		p.m.dwItemData = (ULONG_PTR)Event<O>::Do;			
		p.m.wID = p.m.dwItemData & 0xffff;
		p.m.fState = EnableMenuInit<O>()(p.hWnd);
		InsertMenuItemW(p.h, p.Index()++, false, &p.m);
		return true;
	}
};
template<int N, class P>struct __insert_menu__<Separator<N>, P>
{
	typedef Separator<N> O;
	bool operator()(P &p)
	{	
		p.m.fMask = MIIM_TYPE | MIIM_DATA | MIIM_ID | MIIM_STATE;
		p.m.hSubMenu = NULL;		
		unsigned t = p.m.fType;
		p.m.fType = MFT_SEPARATOR;
		InsertMenuItemW(p.h, p.Index()++, false, &p.m);
		p.m.fType = t;
		return true;
	}
};
template<class S, class P>struct __insert_item_menu__<SubMenu<S>, P>
{
	typedef SubMenu<S> O;
	bool operator()(P &p)
	{      
		if(ReturnItemMenu<O>()())
		{
			__insert_menu__<O, P>()(p);
			return true;
		}
		return false;
	}
};
template<class P, int N>struct __insert_item_menu__<Separator<N>, P>
{	
	bool operator()(P &p)
	{        	
		unsigned t = p.m.fType;
		p.m.fType = MFT_SEPARATOR;
		InsertMenuItemW(p.h, p.Index()++, false, &p.m);
		p.m.fType = t;
		return true;
	}
};
template<class T>void EnableMenu(HWND h, bool state = true)
{
	MENUITEMINFO mii = {};
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	HMENU hMenu = GetMenu(h);
	static const unsigned short id = (unsigned short)Event<T>::Do;
	if(GetMenuItemInfo(hMenu, id, false, &mii))
	{
		mii.fState = state ? MFS_ENABLED : MFS_DISABLED;
		SetMenuItemInfo(hMenu, id, false, &mii);
		DrawMenuBar(h);
	}	
}
template<class T>void ChangeTextSubMenu(HWND h, wchar_t *text)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;
	mii.dwTypeData = text;
	SetMenuItemInfo(GetMenu(h), (unsigned short)Event<T>::Do, false, &mii);
}

void EventDo(TCommand &m);
LRESULT EventDo(TNotify &m);

#define MENU_TEXT(txt, item)template<>struct NameMenu<item >{wchar_t *operator()(HWND){return (wchar_t *)txt;}};
#define MENU_ITEM(txt, item) MENU_TEXT(txt, MenuItem<item>) template<>struct Event<MenuItem<item> >:item{};

template<class T>void CheckMenu(HWND h, bool state = true)
{
	MENUITEMINFO mii = {};
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	HMENU hMenu = GetMenu(h);
	static const unsigned short id = (unsigned short)(LONG_PTR)Event<T>::Do;
	if(GetMenuItemInfo(hMenu, id, false, &mii))
	{
		mii.fState = state ? MFS_CHECKED : MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, id, false, &mii);
	}
}

template<class T>bool SwitchMenu(HWND h)
{
	MENUITEMINFO mii = {};
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	HMENU hMenu = GetMenu(h);
	static const unsigned short id = (unsigned short)(LONG_PTR)Event<T>::Do;	
	if(GetMenuItemInfo(hMenu, id, false, &mii))
	{
		bool result = 0 != (mii.fState & MFS_CHECKED );
		mii.fState = !result ? MFS_CHECKED : MFS_UNCHECKED;
		SetMenuItemInfo(hMenu, id, false, &mii);
		return !result;
	}
	return false;
}
template<class T>bool TestMenu(HWND h)
{
	MENUITEMINFO mii = {};
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	HMENU hMenu = GetMenu(h);
	static const unsigned short id = (unsigned short)(LONG_PTR)Event<T>::Do;
	if (GetMenuItemInfo(hMenu, id, false, &mii))
	{
		return 0 != (mii.fState & MFS_CHECKED);
	}
	return false;
}

#undef index