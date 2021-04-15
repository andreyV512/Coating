#include "regexp.h"
#include <regex>

bool reqexp::GetFileNameFromPath(wchar_t *path, int count)
{
    std::wstring str = path;

    std::wregex r(L"[ \\w]+(?=(\\.[ \\w]+)$)");
    std::wsmatch m;
    if (std::regex_search(str, m, r))
    {
        wcscpy_s(path, count, m[0].str().c_str());
        return true;
    }
    return false;
}
