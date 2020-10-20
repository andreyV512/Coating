#pragma once
namespace Store
{
	void Save(wchar_t *path);
	bool Load(wchar_t *path);
	void Archive();
	void Zip(wchar_t *path);
	void Unzip(wchar_t *path);
}