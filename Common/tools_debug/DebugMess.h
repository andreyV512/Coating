#pragma once
#define XDEBUG
#ifdef XDEBUG
#include <windows.h>
#include <typeinfo> 

//(int*){,,msvcr100d.dll}__crtBreakAlloc

//#ifdef _DEBUG
//    #define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//    // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//    // allocations to be of _CLIENT_BLOCK type
//#else
//    #define new new
//#endif

//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(196);

//_CrtMemState s1, s2, s3;
	//_CrtMemCheckpoint( &s1 );
	//test code
	//_CrtMemCheckpoint( &s2 );

	//if ( _CrtMemDifference( &s3, &s1, &s2) ) 
	//	_CrtMemDumpStatistics( &s3 );

//_CrtDumpMemoryLeaks(); print id debug window

struct TMapData
{
	LONG head;
	LONG tail;
	char data[256][512];
};
class DebugMess
{	
	HANDLE hMapFile;
    TMapData *map;
	DebugMess(void);
	DebugMess &operator=(const DebugMess &);
public:	
	class Initialization;
	void Destroy(void);
	void print(const char *, ...);
};

class ViewerDebugMess
{
	HANDLE hMapFile;
    TMapData *map;
public:
	ViewerDebugMess(void);
public:
	~ViewerDebugMess(void);
	void get(void(*)(char *));
};

extern DebugMess &debug;
#define dprint debug.print
#define zprint(mess, ...)debug.print("%s "##mess, __FUNCTION__, ##__VA_ARGS__)
#define xprint(mess, ...)debug.print("%s "##mess, __FUNCTION__, ##__VA_ARGS__)
#else
#define dprint
#define zprint
#define xprint
#endif
