#include "AScanAuto.h"

AScanAuto::AScanAuto(): obj(nullptr), terminate(false){}
AScanAuto::~AScanAuto() { Stop(); }
void AScanAuto::Init() {}

DWORD WINAPI __run__(PVOID p)
{
	((AScanAuto *)p)->Run();
	return 0;
}
void AScanAuto::Start() 
{
	Init();
	hTresh = CreateThread(NULL, 0, __run__, this, 0, NULL);
}
void AScanAuto::Stop() 
{
	terminate = true;
	WaitForSingleObject(hTresh, INFINITE);
}
void AScanAuto::Run()
{
	while (!terminate)
	{
	}
}