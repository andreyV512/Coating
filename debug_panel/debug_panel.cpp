// debug_panel.cpp : Defines the entry point for the console application.
//
#include <clocale>
#include <stdio.h>
#include "tools_debug/DebugMess.h"

void Print(char *txt)
{
	printf(txt);
}

int main()
{
	setlocale(LC_CTYPE, "rus"); 
	ViewerDebugMess x;
	printf("Панель для вывода отладочной информации\n");
	for(;;)
	{
		x.get(Print);
	}
	return 0;
}

