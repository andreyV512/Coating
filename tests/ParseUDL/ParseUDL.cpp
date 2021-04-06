#include <iostream>
#include <fstream>
#include <filesystem>



int main()
{
    const wchar_t* path = L".\\connection.udl";
    const wchar_t* s = L"Provider = SQLOLEDB.1; Integrated Security = SSPI; Persist Security Info = False; Initial Catalog = StoreBase; Data Source = (local)";
    if (!std::filesystem::exists(path))
    {
        std::wofstream fs(path);
        fs << L"[oledb]" << std::endl;
        fs << L"; Everything after this line is an OLE DB initstring" << std::endl;
        fs << s << std::endl;
        fs.close();
    }
    else
    {
        std::cout << "err\n";
    }
}

