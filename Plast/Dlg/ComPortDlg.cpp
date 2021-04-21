#include "Dlg.h"
#include "DlgTemplates\ParamDlgNew.h"

template<int N>struct BaudRateX;
#define BAUDRATE(n)template<>struct BaudRateX<n>\
{\
	static const int value = CBR_##n;\
	wchar_t *Name(){return L#n;}\
};

BAUDRATE(110)
BAUDRATE(300)
BAUDRATE(600)
BAUDRATE(1200)
BAUDRATE(2400)
BAUDRATE(4800)
BAUDRATE(9600)
BAUDRATE(14400)
BAUDRATE(19200)
BAUDRATE(38400)
BAUDRATE(57600)
BAUDRATE(115200)
BAUDRATE(128000)
BAUDRATE(256000)

typedef Vlst<
	BaudRateX<110	>
	, BaudRateX<300	>
	, BaudRateX<600	>
	, BaudRateX<1200  >
	, BaudRateX<2400  >
	, BaudRateX<4800  >
	, BaudRateX<9600  >
	, BaudRateX<14400 >
	, BaudRateX<19200 >
	, BaudRateX<38400 >
	, BaudRateX<57600 >
	, BaudRateX<115200>
	, BaudRateX<128000>
	, BaudRateX<256000>
>baud_rate_list;

template<int >struct ParityX;
#define PARITY(n)template<>struct ParityX<n>\
{\
	static const int value = n;\
	wchar_t *Name(){return L#n;}\
};

PARITY(EVENPARITY)
PARITY(MARKPARITY)
PARITY(NOPARITY)
PARITY(ODDPARITY)
PARITY(SPACEPARITY)

typedef Vlst<
	ParityX<EVENPARITY 	 >
	, ParityX<MARKPARITY >
	, ParityX<NOPARITY	 >
	, ParityX<ODDPARITY  >
	, ParityX<SPACEPARITY>
>baud_parity_list;

template<int >struct StopBitsX;
#define STOPBITS(n)template<>struct StopBitsX<n>\
{\
	static const int value = n;\
	wchar_t *Name(){return L#n;}\
};

STOPBITS(ONESTOPBIT)
STOPBITS(ONE5STOPBITS)
STOPBITS(TWOSTOPBITS)

typedef Vlst<
	StopBitsX<ONESTOPBIT	>
	, StopBitsX<ONE5STOPBITS	>
	, StopBitsX<TWOSTOPBITS 	>
>baud_stop_bits_list;

template<class O, class P>struct __set_param__
{
	void operator()(P &p)
	{
		ComboBox_AddString(p, O().Name());
	}
};

template<class List>struct __data__
{
	typedef List list;
	int id;
};

template<class O, class P>struct __get_param__
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<P::list, O>::value == p.id)
		{
			p.id = O::value;
			return false;
		}
		return true;
	}
};

template<class O, class P>struct __current_value__
{
	bool operator()(P& p)
	{
		if (O::value == p.id)
		{
			p.id = VL::IndexOf<P::list, O>::value;
			return false;
		}
		return true;
	}
};				 

template<class Dlg>struct __data_from_widget__<Dialog::DlgItem2<StopBits, Dlg>, int>
{
	int operator()(Dialog::DlgItem2<StopBits, Dlg>& o)
	{
		__data__<baud_stop_bits_list> data;
		data.id = ComboBox_GetCurSel(o.hWnd);
		VL::find<baud_stop_bits_list, __get_param__>()(data);
		return data.id;
	}
};
template<>struct FillComboboxList<StopBits>
{
	void operator()(HWND h, StopBits&)
	{
		VL::foreach<baud_stop_bits_list, __set_param__>()(h);
	}
};

template<>struct CurrentValue<StopBits>
{
	void operator()(HWND h, StopBits&)
	{
		__data__<baud_stop_bits_list> data;
		data.id = Singleton<ComPortTable>::Instance().items.get<StopBits>().value;
		VL::find<baud_stop_bits_list, __current_value__>()(data);
		ComboBox_SetCurSel(h, data.id);
	}
};

template<class Dlg>struct __data_from_widget__<Dialog::DlgItem2<Parity, Dlg>, int>
{
	int operator()(Dialog::DlgItem2<Parity, Dlg>& o)
	{
		__data__<baud_parity_list> data;
		data.id = ComboBox_GetCurSel(o.hWnd);
		VL::find<baud_parity_list, __get_param__>()(data);
		return data.id;
	}
};

template<>struct FillComboboxList<Parity>
{
	void operator()(HWND h, Parity&)
	{
		VL::foreach<baud_parity_list, __set_param__>()(h);
	}
};

template<>struct CurrentValue<Parity>
{
	void operator()(HWND h, Parity&)
	{
		__data__<baud_parity_list> data;
		data.id = Singleton<ComPortTable>::Instance().items.get<Parity>().value;
		VL::find<baud_parity_list, __current_value__>()(data);
		ComboBox_SetCurSel(h, data.id);
	}
};

template<>struct FillComboboxList<BaudRate>
{
	void operator()(HWND h, BaudRate&)
	{
		VL::foreach<baud_rate_list, __set_param__>()(h);
	}
};

template<>struct CurrentValue<BaudRate>
{
	void operator()(HWND h, BaudRate&)
	{
		__data__<baud_rate_list> data;
		data.id = Singleton<ComPortTable>::Instance().items.get<BaudRate>().value;
		VL::find<baud_rate_list, __current_value__>()(data);
		ComboBox_SetCurSel(h, data.id);
	}
};

PARAM_TITLE(ComPortAddr, L"Адрес ком-порта")
PARAM_TITLE(BaudRate, L"Скорость")
PARAM_TITLE(Parity, L"Чётность")
PARAM_TITLE(StopBits, L"Количество бит стоп")

DO_NOT_CHECK(ComPortAddr)
DO_NOT_CHECK(BaudRate)
DO_NOT_CHECK(Parity)
DO_NOT_CHECK(StopBits)

template<>struct DlgSubItems<ComPortAddr, int> : UpDownSubItem<ComPortAddr> {};
template<>struct DlgSubItems<BaudRate, int> : ComboBoxSubItem<BaudRate> {};
template<>struct DlgSubItems<Parity, int> : ComboBoxSubItem<Parity> {};
template<>struct DlgSubItems<StopBits, int> : ComboBoxSubItem<StopBits> {};


void ComPortDlg::Do(HWND h)
{
	if (Dialog::Templ < ParametersBase, ComPortTable>(Singleton<ComPortTable>::Instance()).Do(h, (wchar_t*)L"Настройки ком-порта"))
	{
	}
}