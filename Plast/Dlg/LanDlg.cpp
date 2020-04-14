#include "Dlg.h"
#include "App/App.h"
#include "App/AppBase.h"
#include "DlgTemplates/ParamDlgNew.h"

PARAM_TITLE(PacketSize, L"Размер пакета данных")
PARAM_TITLE(Gain0, L"Усиление канала данных")
PARAM_TITLE(SyncLevel, L"Уровень синхронизации")
PARAM_TITLE(SyncGain, L"Усиление канала синхронизации")
PARAM_TITLE(StartDelay, L"Задержка при старте")
//---------------------------------------------------------------
PARAM_TITLE(Frequency, L"Частота сбора данных(МГц)")
PARAM_TITLE(NumberPackets, L"Количество пакетов данных")

PARAM_TITLE(SyncInput, L"Вход синхронизации открыт")
PARAM_TITLE(MeasurementInput, L"Измерительный вход открыт")
PARAM_TITLE(SynchronizationEdge, L"Синхронизация по фронту")
//.............................................................
MIN_VALUE(SyncLevel, 1)
MAX_VALUE(SyncLevel, 255)

MIN_VALUE(StartDelay, -1)
MAX_VALUE(StartDelay, 999)

DO_NOT_CHECK(PacketSize)
DO_NOT_CHECK(Gain0)
DO_NOT_CHECK(SyncGain)
//---------------------------------------------------------------------
DO_NOT_CHECK(Frequency)

MIN_VALUE(NumberPackets, 0)
MAX_VALUE(NumberPackets, 1000000 / 986 / App::count_sensors)

//..................................................................
template<>struct DlgSubItems<PacketSize, int> : ComboBoxSubItem<PacketSize> {};
const int PacketSizeData[] = { 474, 986 };
template<>struct FillComboboxList<PacketSize>
{
	void operator()(HWND h, PacketSize &t)
	{
		wchar_t buf[32];
		for (int i = 0; i < dimention_of(PacketSizeData); ++i)
		{
			ComboBox_AddString(h, _itow(PacketSizeData[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<PacketSize>
{
	void operator()(HWND h, PacketSize &t)
	{
		for (int i = 0; i < dimention_of(PacketSizeData); ++i)
		{
			if (PacketSizeData[i] == Singleton<LanTable>::Instance().items.get<PacketSize>().value)
			{
				ComboBox_SetCurSel(h, i);
				break;
			}
		}
	}
};
//..................................................................
template<>struct DlgSubItems<Gain0, int> : ComboBoxSubItem<Gain0> {};
const int Gain0Data[] = { 1, 2, 5, 10, 20, 40, 100, 200 };
template<>struct FillComboboxList<Gain0>
{
	void operator()(HWND h, Gain0 &t)
	{
		wchar_t buf[32];
		for (int i = 0; i < dimention_of(Gain0Data); ++i)
		{
			ComboBox_AddString(h, _itow(Gain0Data[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<Gain0>
{
	void operator()(HWND h, Gain0 &t)
	{
		for (int i = 0; i < dimention_of(Gain0Data); ++i)
		{
			if (Gain0Data[i] == Singleton<LanTable>::Instance().items.get<Gain0>().value)
			{
				ComboBox_SetCurSel(h, i);
				break;
			}
		}
	}
};
//-------------------------------------------------------------------
template<>struct DlgSubItems<Frequency, int> : ComboBoxSubItem<Frequency> {};
const int FrequencyData[] = { 25, 50 };
template<>struct FillComboboxList<Frequency>
{
	void operator()(HWND h, Frequency &t)
	{
		wchar_t buf[32];
		for (int i = 0; i < dimention_of(FrequencyData); ++i)
		{
			ComboBox_AddString(h, _itow(FrequencyData[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<Frequency>
{
	void operator()(HWND h, Frequency &t)
	{
		for (int i = 0; i < dimention_of(FrequencyData); ++i)
		{
			if (FrequencyData[i] == Singleton<LanTable>::Instance().items.get<Frequency>().value)
			{
				ComboBox_SetCurSel(h, i);
				break;
			}
		}
	}
};
//..................................................................
template<>struct DlgSubItems<SyncGain, int> : ComboBoxSubItem<SyncGain> {};
const int SyncGainData[] = { 1, 10 };
template<>struct FillComboboxList<SyncGain>
{
	void operator()(HWND h, SyncGain &t)
	{
		wchar_t buf[32];
		for (int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			ComboBox_AddString(h, _itow(SyncGainData[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<SyncGain>
{
	void operator()(HWND h, SyncGain &t)
	{
		for (int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			if (SyncGainData[i] == Singleton<LanTable>::Instance().items.get<SyncGain>().value)
			{
				ComboBox_SetCurSel(h, i);
				break;
			}
		}
	}
};

void LanDlg::Do(HWND h)
{
	if (Dialog::Templ<ParametersBase, LanTable
	>(
		Singleton<LanTable>::Instance()
	).Do(h, (wchar_t *)L"Настройка аналоговой платы"))
	{
	}
}