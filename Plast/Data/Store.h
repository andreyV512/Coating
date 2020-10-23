#pragma once
#include <ATLComTime.h>
namespace Store
{
	void Save(wchar_t *path);
	bool Load(wchar_t *path);
	void Archive(COleDateTime &tme);
}