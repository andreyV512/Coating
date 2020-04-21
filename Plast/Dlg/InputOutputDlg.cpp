﻿#include "Dlg.h"
#include "DlgTemplates\ParamDlgNew.h"

#define Param(n, txt)\
MIN_EQUAL_VALUE(n, 1)\
MAX_EQUAL_VALUE(n, 32)\
PARAM_TITLE(n, txt)\
template<>struct DlgSubItems<n, unsigned>: UpDownSubItem<n>{};


template<class O, class P>struct __unshift__
{
	void operator()(O &o, P &p)
	{
		unsigned val = p.get<O>().value;
		unsigned offs = 1;
		while(val >>= 1) ++offs;
		o.value = offs;
	}
};

Param(iCU            , L"Сигнал ЦЕПИ УПРАВЛЕНИЯ")
Param(iIn            , L"Труба на входе")
Param(iOut           , L"Труба на выходе")

Param(oAutomat, L"Автомат")
Param(oSupply , L"Подвод")
Param(oMark     , L"Отметка")
#undef Param

typedef Dialog::Templ<ParametersBase, InputBitsTable, InputBitsTable::items_list, 350> TInputBitDlg;
template<class P>struct __data_from_widget__<Dialog::DlgItem2<P, TInputBitDlg>, unsigned>
{
    unsigned operator()(Dialog::DlgItem2<P, TInputBitDlg> &o)
	{
		wchar_t buf[128];
		GetWindowText(o.hWnd, buf, dimention_of(buf));
		return 1 << (Wchar_to<unsigned>()(buf) - 1);
	}
};

void InputBitDlg::Do(HWND h)
{
	InputBitsTable par;
	VL::foreach<InputBitsTable::items_list, __unshift__>()(par.items, Singleton<InputBitsTable>::Instance().items);
	if(TInputBitDlg(par).Do(h, (wchar_t *)L"Смещение входных портов"))
	{
	}
}

typedef Dialog::Templ<ParametersBase, OutputBitsTable, OutputBitsTable::items_list, 300> TOutputBitDlg;
template<class P>struct __data_from_widget__<Dialog::DlgItem2<P, TOutputBitDlg>, unsigned>
{
    unsigned operator()(Dialog::DlgItem2<P, TOutputBitDlg> &o)
	{
		wchar_t buf[128];
		GetWindowText(o.hWnd, buf, dimention_of(buf));
		return 1 << (Wchar_to<unsigned>()(buf) - 1);
	}
};

void OutputBitDlg::Do(HWND h)
{
	OutputBitsTable par;
	VL::foreach<OutputBitsTable::items_list, __unshift__>()(par.items, Singleton<OutputBitsTable>::Instance().items);
	if(TOutputBitDlg(par).Do(h, (wchar_t *)L"Смещение выходных портов"))
	{
	}
}

DO_NOT_CHECK(NamePlate1730)
PARAM_TITLE(NamePlate1730, L"Дескриптор")

void DiscriptorBitDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, NamePlate1730ParametersTable
	>(Singleton<NamePlate1730ParametersTable>::Instance()).Do(h, (wchar_t *)L"Дескриптор платы"))
	{
	}
}