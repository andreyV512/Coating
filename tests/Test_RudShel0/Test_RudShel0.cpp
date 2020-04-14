// Test_RudShel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lan\Lan.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(lan.Init()) 
	{
		lan.InitParameters();
		return 0;
	}
	printf("Error Lan\n");
	getchar();
	return 0;
}

