#include "Dlg.h"
#include "Windows/MainWindow/MainWindow.h"
#include "DlgTemplates/ParamDlgNew.h"

namespace
{
	struct AddOkBtn;
	typedef Dialog::Templ<ParametersBase, ParametersTable
		, Vlst<NameParam>
		, 400
		, Vlst<AddOkBtn, CancelBtn>
	> Add_Typesize;
	struct AddOkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return (wchar_t *)L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<Owner::Base, Owner::Table>()(h))
			{
				wchar_t buf[128];
				
				GetWindowText(owner.items.get<Dialog::DlgItem2<NameParam, Add_Typesize> >().hWnd, buf, dimention_of(buf));
				if(0 == buf[0] || 0 == wcscmp(L"NONAME", buf))
				{
					MessageBox(h, L"Введите название типоразмера", L"Ошибка!!!", MB_ICONERROR);
					return;
				}				
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					owner.table.items.get<NameParam>().value = buf;
					int id = Select<Owner::Table>(base).eq<NameParam>(owner.table.items.get<NameParam>().value).Execute();
					if(0 != id)
					{
						MessageBox(h, L"Название типоразмера есть в базе данных", L"Предупреждение!!!", MB_ICONEXCLAMATION);
					}
					else
					{
						Insert_Into<Owner::Table>(owner.table, base).Execute();//<Owner::Table::items_list>();
						id = Select<Owner::Table>(base).eq<NameParam>(owner.table.items.get<NameParam>().value).Execute();	
						MessageBox(h, L"Типоразмер создан", L"Сообщение!!!", MB_ICONINFORMATION);
					}
					CurrentParametersTable &curr = Singleton<CurrentParametersTable>::Instance();
					curr.items.get<CurrentID>().value = id;
					UpdateWhere<CurrentParametersTable>(curr, base).ID(__id__).Execute();
					//
					MainWindow &o = Singleton<MainWindow>::Instance();
					o.select.AddMenuItem(owner.table.items.get<NameParam>().value.buffer);	
					TCommand c(o.hWnd, 0, 1, o.select.hWnd);
					o.select.Do(c);
				}
				EndDialog(h, TRUE);
			}
		}
	};

	template<class T, class D>struct  __more_than_one__
	{
		bool operator()(unsigned, T &t, D &d)
		{
			++d;
			return d > 1;
		}
	};

	struct DelOkBtn;
	typedef Dialog::Templ<ParametersBase, ParametersTable
		, Vlst<NameParam>, 400, Vlst<DelOkBtn, CancelBtn>> Del_Typesize;

	struct DelOkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return (wchar_t *)L"Удалить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<Owner::Base, Owner::Table>()(h))
			{
				wchar_t buf[128];
				GetWindowText(owner.items.get<Dialog::DlgItem2<NameParam, Del_Typesize> >().hWnd, buf, dimention_of(buf));
				if(0 == buf[0])
				{
					MessageBox(h, L"Введите название типоразмера", L"Ошибка!!!", MB_ICONERROR);
					return;
				}
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					int countItems = 0;
					Select<Owner::Table>(base).ExecuteLoop<__more_than_one__>(countItems);
					if(countItems < 2)
					{
						MessageBox(h, L"В базе должно быть больше одного типоразмера", L"Предупреждение!!!", MB_ICONEXCLAMATION);
						return;
					}					
					wchar_t buf2[128];
					wcsncpy_s(buf2, buf, wcslen(buf));
				
					MainWindow &o = Singleton<MainWindow>::Instance();
					o.select.DelMenuItem(buf);
				
					if(buf[0] != 0)
					{
						NameParam n;
						n.value = buf2;
						Delete<Owner::Table>(base).eq<NameParam>(n.value).Execute();
						n.value = buf;
						int id = Select<Owner::Table>(base).eq<NameParam>(n.value).Execute();
						if(id)
						{
							Update<CurrentParametersTable>(base).set<CurrentID>(id).Where().ID(__id__).Execute();
							AppBase::InitTypeSizeTables(base);
							
							TCommand c(o.hWnd, 0, 1, o.select.hWnd);
							o.select.Do(c);
						}
					}
				}
				EndDialog(h, TRUE);
			}
		}
	};
}

DO_NOT_CHECK(NameParam)
PARAM_TITLE(NameParam, L"")
template<int N>struct DlgSubItems<NameParam, Holder<N> >: EditItems<NameParam, 380>{};

void AddTypeSizeDlg::Do(HWND h)
{
	ParametersTable &t = Singleton<ParametersTable>::Instance();
	if(Dialog::Templ<ParametersBase, ParametersTable
		, Vlst<NameParam>
		, 400
		, Vlst<AddOkBtn, CancelBtn>
	>(t).Do(h, (wchar_t *)L"Создать типоразмер")
	)
	{}
}

template<>struct Dialog::DlgItem2<NameParam, Del_Typesize>
{
	typedef NameParam T;
	typedef Del_Typesize Owner;
	static const int DY = DlgSubItems<T, typename T::type_value>::DY;
	Owner &owner;
	T &value;
	HWND hWnd;
	DlgItem2(Owner &o) : owner(o), value(o.table.items.get<T>()){}
	void Init(HWND h, int &x, int &width, int &dy)
	{
		hWnd = EditReadOnlyItems<T, 380>().Init(h, x, width, dy, value);
	}
};

void DelTypeSizeDlg::Do(HWND h)
{
	ParametersTable &t = Singleton<ParametersTable>::Instance();
	Del_Typesize(t).Do(h, (wchar_t *)L"Удалить типоразмер");
}

