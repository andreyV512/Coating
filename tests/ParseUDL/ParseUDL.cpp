#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

int main()
{
    //const wchar_t* path = L".\\connection.udl";
    //const wchar_t* s = L"Provider = SQLOLEDB.1; Integrated Security = SSPI; Persist Security Info = False; Initial Catalog = StoreBase; Data Source = (local)";
    //if (!std::filesystem::exists(path))
    //{
    //    std::wofstream fs(path);
    //    fs << L"[oledb]" << std::endl;
    //    fs << L"; Everything after this line is an OLE DB initstring" << std::endl;
    //    fs << s << std::endl;
    //    fs.close();
    //}
    //else
    //{
    //    std::cout << "err\n";
    //}

    std::wstring str = L"c:\\a\\b\\c123\\name123.dat";

    std::wregex r(L"[ \\w]+(?=(\\.[ \\w]+)$)");
    std::wsmatch m;
    if (std::regex_search(str, m, r))
    {
        std::wstring s = m[0];
        const wchar_t* c = s.c_str();
        //std::wcout << s << std::endl;
    }

   // std::cout << m << std::endl;
   /// getchar();

}

