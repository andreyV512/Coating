#include "Store.h"
#include "App/AppBase.h"
#include "Log/LogMessages.h"
#include "window_tool/ProgrammDir.h"
#include "Data/Data.h"
#include "Log/LogBuffer.h"
#include "window_tool/Zip.h"


static const unsigned __magic__ = 0x0a130000;

template<class O, class P>struct __save__
{
	void operator()(O &o, P &f)
	{
		fwrite(&o.value, sizeof(o.value), 1, f);
	}
};

template<class O, class P>struct __save_all__
{
	void operator()(P &p)
	{
		typename O::TItems &o = Singleton<O>::Instance().items;
		VL::foreach<typename O::items_list, __save__>()(o, p);
	}
};

void Store::Save(wchar_t *path)
{
	FILE *f = _wfopen(path, L"wb");
	if (f)
	{
		unsigned magic = __magic__;
		fwrite(&magic, sizeof(magic), 1, f);
		VL::foreach<Vlst<PARAM_ID>, __save_all__>()(f);

		Data::InputData &data = Singleton<Data::InputData>::Instance();

		fwrite(&data.framesCount, sizeof(data.framesCount), 1, f);
		fwrite(&data.strobesTickCount, sizeof(data.strobesTickCount), 1, f);
		fwrite(&data.offsetsTickCount, sizeof(data.offsetsTickCount), 1, f);
		
		fwrite(data.buffer, sizeof(char), data.framesCount, f);
		fwrite(&data.strobesTick, sizeof(unsigned), data.strobesTickCount, f);
		fwrite(&data.offsetsTick, sizeof(unsigned), data.offsetsTickCount, f);

		fclose(f);
	}
}

template<class O, class P>struct __load__
{
	void operator()(O &o, P &p)
	{
		fread(&o.value, sizeof(o.value), 1, p);
	}
};

template<class O, class P>struct __load_all__
{
	void operator()(P &p)
	{
		typename O::TItems &o = Singleton<O>::Instance().items;
		VL::foreach<typename O::items_list, __load__>()(o, p);
	}
};

void Load0(FILE *f)
{
	Log::Mess <LogMess::FileDownloading>();
	VL::foreach<Vlst<PARAM_ID>, __load_all__>()(f);

	Data::InputData &data = Singleton<Data::InputData>::Instance();

	fread(&data.framesCount, sizeof(data.framesCount), 1, f);
	fread(&data.strobesTickCount, sizeof(data.strobesTickCount), 1, f);
	fread(&data.offsetsTickCount, sizeof(data.offsetsTickCount), 1, f);

	fread(data.buffer, sizeof(char), data.framesCount, f);
	fread(data.strobesTick, sizeof(unsigned), data.strobesTickCount, f);
	fread(data.offsetsTick, sizeof(unsigned), data.offsetsTickCount, f);
	fclose(f);
}

bool Store::Load(wchar_t *path)
{
	FILE *f = _wfopen(path, L"rb");
	bool res = false;
	if (f)
	{
		unsigned magic = 0;
		fread(&magic, sizeof(magic), 1, f);
		switch (magic)
		{
		case 0 + __magic__:
			Load0(f);
			res = true;
			break;
		}
		if(!res)fclose(f);
	}

	if (res)
		Log::Mess <LogMess::FileDownloaded>();
	else
		Log::Mess <LogMess::FormatDownloadedFileNoSupported>();

	return res;
}

void Store::Archive(COleDateTime &tme)
{
	

	StoreTable::TItems &st = Singleton<StoreTable>::Instance().items;

	if (st.get<StoreFileOn>().value)
	{
		ProgrammDir pd;

		wsprintf(pd.tail, L"\\..\\store\\%02d%02d%02d%02d%02d%02d.dat"
			, tme.GetYear() - 2000
			, tme.GetMonth()
			, tme.GetDay()
			, tme.GetHour()
			, tme.GetMinute()
			, tme.GetSecond()
		);
		Store::Save(pd.path);
		Zip::RemoveZipFilesInDirectory(st.get<CountStoredFiles>().value);
		Zip::ZipAsyncAll();
	}
}

