#include <atlsimpstr.h>
#include "LogBuffer.h"
#include "tools_debug/DebugMess.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include <vector>
#include <algorithm>
wchar_t name[] = L"Parallel hatch in log space ";

#define d_mess

namespace LogSpace{
	static const unsigned SizeBuffer = 1024;

	struct TMapData
	{
		volatile ULONG head;
		volatile ULONG tail;
		Log::TData data[SizeBuffer];
	};
}
LONG Log::lastMessage = -1;
struct Log_Inner
{
	std::vector<unsigned> skip;
	HANDLE hMapFile;
	LogSpace::TMapData *map;
	bool IsRow(unsigned row, Log::TData *&d)
	{
		if(map->head - map->tail >= LogSpace::SizeBuffer) map->tail = map->head - LogSpace::SizeBuffer;
		if(map->head - map->tail > row)
		{
			unsigned i = map->head - row;
			i %= LogSpace::SizeBuffer; 
			d = &map->data[i];
			return true;
		}
		return false;
	}

	bool IsRowTest(unsigned row, Log::TData *&d, unsigned &offset)
	{
		if (map->head - map->tail >= LogSpace::SizeBuffer) map->tail = map->head - LogSpace::SizeBuffer;
		unsigned i = 0 == row ? map->head : offset;
		for (; i >= map->tail; --i)
		{
			unsigned k = i % LogSpace::SizeBuffer;
			d = &map->data[k];
			if (skip.end() != std::find(skip.begin(), skip.end(), d->id))
			{
				offset = i;
				return true;
			}
		}
		return false;
	}

	void Get(unsigned i, Log::TData *&d)
	{
		d = &map->data[i % LogSpace::SizeBuffer];
	}
	Log_Inner()
	{
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security 
			PAGE_READWRITE,          // read/write access
			0,                       // max. object size 
			sizeof(LogSpace::TMapData),                // buffer size  
			name);                 // name of mapping object

		unsigned res = GetLastError();
		map = (LogSpace::TMapData *) MapViewOfFile(hMapFile,   // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0,                   
			0,                   
			sizeof(LogSpace::TMapData));           

		if (map == NULL) 
		{ 
			d_mess("Could not map view of file (%d).\n", 
				GetLastError()); 
			return;
		}
		Performance::Init();
		if(0 == res)
		{
			map->head = 0;
			map->tail = 0;
		}
		else
		{
			if(map->head - map->tail > LogSpace::SizeBuffer) map->tail = map->head - LogSpace::SizeBuffer;
		}
		inner.map->data[map->head % LogSpace::SizeBuffer].time = Performance::Counter();
	}
} inner;

int Log::ID = LogMess::MAX_MESS_ID;

void Log::Insert(unsigned id, double val, bool b)
{
	Log::ID = id;
	LONG i = _InterlockedIncrement((LONG *)&inner.map->head);
	i &= LogSpace::SizeBuffer - 1;
	if(b) InterlockedExchange(&lastMessage, i);
	inner.map->data[i].id = id;
	inner.map->data[i].time = Performance::Counter();
	inner.map->data[i].value = val;
}

bool Log::IsRow(unsigned row, TData *&d)
{
	return inner.IsRow(row, d);
}

int Log::LastMessageIndex()
{
	return inner.map->head;
}

bool Log::LastMessage(TData *&d)
{
	int t = InterlockedExchange(&lastMessage, -1);
	if(-1 == t) return false;
	inner.Get(t, d);
	return true;
}

 void Log::TailMessage(TData *&d)
 {
	 inner.Get(lastMessage, d);
 }

 void Log::SetSkip(unsigned id)
 {
	 inner.skip.push_back(id);
 }

 void Log::CleanSkip()
 {
	 inner.skip.clear();
 }

 bool Log::IsRowTest(unsigned row, TData *&d, unsigned &offset)
 {
	 return inner.IsRowTest(row, d, offset);
 }
