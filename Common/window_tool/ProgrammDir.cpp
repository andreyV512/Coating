#include "ProgrammDir.h"
#include <windows.h>
#include <Shlwapi.h>
#include "templates/typelist.hpp"

ProgrammDir::ProgrammDir()
{
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	tail = path + wcslen(path);
}
