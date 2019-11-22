#pragma once
#include "window_tool/TEvent.h"
#include <CommCtrl.h>
#include <vector>
#include <string>
class SelectUsers : public TEvent
{
public:
	struct Item
	{
		int id;
		std::wstring name;
		int tabelNum;
	};
	std::vector<Item> items;
	std::vector<Item> delItems;
public:
	struct OkButton : TEvent
	{
		SelectUsers &owner;
		OkButton(SelectUsers &);
		void Do(TCommand &);
	} okButton;
	HWND hWnd;
	SelectUsers();
	void Create(HWND);
	void Size(int, int, int, int);
	LRESULT Do(TNotify &);
	void RClick(LPNMITEMACTIVATE);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
};