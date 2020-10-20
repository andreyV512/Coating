#pragma once

class ProgrammDir
{
public:
	wchar_t path[2048];
	wchar_t *tail;
	ProgrammDir();
};