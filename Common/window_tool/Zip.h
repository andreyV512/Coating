#pragma once
namespace Zip
{
	void Zip(wchar_t *nameFile);
	void ZipCurentDir(wchar_t *nameFile);
	void Unzip(wchar_t *nameFile);
	void UnzipCurrentDir(wchar_t *nameFile);
	void ZipAll();
	void ZipAsyncAll();
	int CountZipFilesInDirectory();
	void RemoveZipFilesInDirectory(int maxCountFiles);
}