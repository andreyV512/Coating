#pragma once
#include <Windows.h>

class ComPortHandler
{	
public:
	inline static int status;
	inline static char abonent;
	virtual void operator()(unsigned char (&input)[1024], int){}
};

class ComPort
{
private:
    //void(*receive)(ComPort&, int);
	//void(*timeout)(ComPort &)
	HANDLE hCom, hThread, hEvent, hComEvent;
	OVERLAPPED inputOverlapped, outputOverlapped;
	static DWORD WINAPI Run(LPVOID);
	ComPortHandler *comPortHandler = NULL;
	int current = 0;
	unsigned char input[1024], output[1024];
public:
	ComPort();
	~ComPort();
	bool Open(int numberComPort, int speed, int parity = NOPARITY, int stopBits = ONESTOPBIT);
	void Close();
	bool IsOpen();
	//void Write(unsigned char *, int, ComPortHandler* = NULL);
	//void Write(unsigned char *, int, void(*receive)(ComPort &, int) = NULL, void(*timeout)(ComPort &) = NULL);
	void Write(unsigned char *, int);
	//void SetReceiveHandler(void(*)(ComPort &, unsigned char(&)[1024], int));
	void SetReceiveHandler(ComPortHandler *comPortHandler);
private:
	void Do();
};