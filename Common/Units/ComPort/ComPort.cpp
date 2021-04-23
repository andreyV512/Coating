#include "ComPort.h"
#include <stdio.h>
#include "tools_debug/DebugMess.h"

ComPort::ComPort()
	: hCom(INVALID_HANDLE_VALUE)
{
	ZeroMemory(&inputOverlapped, sizeof(inputOverlapped));
	ZeroMemory(&outputOverlapped, sizeof(outputOverlapped));
	
	inputOverlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	outputOverlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hThread = CreateThread(NULL, 0, Run, this, CREATE_SUSPENDED, NULL);
}

ComPort::~ComPort()
{
	Close();
	CloseHandle(hThread);
	CloseHandle(inputOverlapped.hEvent);
	CloseHandle(outputOverlapped.hEvent);
}

DWORD WINAPI ComPort::Run(LPVOID p)
{
	((ComPort *)p)->Do();
	return 0;
}

bool ComPort::Open(int numberComPort, int speed, int parity, int stopBits)
{
	wchar_t buf[64];
	wsprintf(buf, L"\\\\.\\COM%d", numberComPort);
	hCom = CreateFile(
		buf
		, GENERIC_READ | GENERIC_WRITE
		, 0
		, NULL
		, OPEN_EXISTING
		, FILE_FLAG_OVERLAPPED
		, 0
		);

	DCB dcb;
	GetCommState(hCom,&dcb);
	dcb.BaudRate = speed;
	dcb.Parity = parity;
	dcb.StopBits = stopBits;
	dcb.ByteSize = 8;
	SetCommState(hCom,&dcb);

	if(!SetCommState(hCom,&dcb))
	{
		int err = GetLastError();
		dprint("SetCommState err %d\n", err);
	}

	COMMTIMEOUTS t;
	GetCommTimeouts(hCom,&t);
	t.ReadIntervalTimeout         = 1;
	t.ReadTotalTimeoutMultiplier  = 0;
	t.ReadTotalTimeoutConstant    = 10;
	t.WriteTotalTimeoutMultiplier = 1;
	t.WriteTotalTimeoutConstant   = 1;
	if(!SetCommTimeouts(hCom,&t))
	{
		int err = GetLastError();
		dprint("SetCommTimeouts err %d\n", err);
	}

	if(!SetCommMask(hCom, EV_RXCHAR))
	{
		int err = GetLastError();
		dprint("SetCommMask err %d\n", err);
	}

	while(ResumeThread(hThread));

	return IsOpen();
}

void ComPort::Close()
{
	SuspendThread(hThread);
	if(IsOpen())CloseHandle(hCom);
	hCom = INVALID_HANDLE_VALUE;
}

bool ComPort::IsOpen()
{
	return hCom != INVALID_HANDLE_VALUE;
}

//void ComPort::Write(unsigned char *buf, int len, ComPortHandler* comPortHandler)
//{
//	if(len > 0 && NULL != buf)
//	{
//		this->comPortHandler = comPortHandler;
//		DWORD toWrite;
//		current = 0;
//		if(len > 1024) len = 1024;
//		memmove(output, buf, len);
//		WriteFile(hCom, output, len, &toWrite, &outputOverlapped);
//	}
//}


//void ComPort::Write(unsigned char *buf, int len, void(*stopPros)(ComPort &, unsigned char(&)[1024], int))
//{	
//	if(len > 0 && NULL != buf)
//	{
//		if(NULL != stopPros)ptrStopProc = stopPros;
//		DWORD toWrite;
//		if(len > 1024) len = 1024;
//		memmove(output, buf, len);
//		WriteFile(hCom, output, len, &toWrite, &outputOverlapped);
//	}
//}
//
void ComPort::Write(unsigned char *buf, int len)
{
	if(len > 0 && NULL != buf)
	{
		DWORD toWrite;
		current = 0;
		if(len > 1024) len = 1024;
		memmove(output, buf, len);
		WriteFile(hCom, output, len, &toWrite, &outputOverlapped);
	}
}
//
void ComPort::SetReceiveHandler(ComPortHandler *comPortHandler)
{
	this->comPortHandler = comPortHandler;
}

void ComPort::Do()
{	
	DWORD mask = 0;
	while(true)
	{
		WaitCommEvent(hCom, &mask, &inputOverlapped);
		switch(WaitForSingleObject(inputOverlapped.hEvent, 10))
		{
		case WAIT_OBJECT_0:
			{
				if(mask & EV_RXCHAR)
				{
					DWORD toRead;
					int count = sizeof(input) - current;
					if(count <= 0)
					{
						current = 0;
						count = sizeof(input);
					}
					if (ReadFile(hCom, &input[current], count, &toRead, &inputOverlapped))
					{
						GetOverlappedResult(hCom, &inputOverlapped, &toRead, TRUE);
						if (toRead > 0)
						{
							current += toRead;
						}
					}
				}
			}
			break;
		case WAIT_TIMEOUT:
			{
				//if(current > 0)
				//{
			    //    void(*t)(ComPort&, unsigned char(&)[1024], int) = ptrStopProc;
				//	if(NULL != t)(*t)(*this, input, current);
				//	current = 0;
				//}
				ComPortHandler *t = comPortHandler;
			    if(NULL != t) (*t)(input, current);
			}
			break;
		}
	}	
}

