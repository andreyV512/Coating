#pragma once
#include "templates/templates.hpp"

#define DEFINE_PARAM(z, type, def_val) struct z\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z;}\
	z() : value(def_val), default_value(def_val) {}\
};
struct Unique{};
#define DEFINE_UNIQUE_PARAM(z, type, def_val) struct z\
{\
	typedef Unique UNIQUE;\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z;}\
	z() : value(def_val), default_value(def_val) {}\
};

template<class T>struct ID
{
	typedef typename T::__ID_template_must_be_overridded__ noused;
};

#define DEFINE_PARAM_ID(z, type, def_val) template<> struct ID<z>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z##"ID";}\
	ID() : value(def_val), default_value(def_val) {}\
};

#define DEFINE_PARAM_NUM(z, n, type, def_val) template<>struct z<n>\
{\
	static const int ID = n;\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L###z##L###n;}\
	z() : value(def_val), default_value(def_val) {}\
};

#define STR_UNIQUE_PARAM(namePar, len, txt)struct namePar\
{\
	typedef Unique UNIQUE;\
	typedef Holder<len> type_value;\
	type_value value;\
	type_value default_value;\
	const wchar_t *name(){return L###namePar;}\
	namePar()\
	{\
		default_value = (wchar_t *)txt;\
		value = (wchar_t *)default_value;\
	}\
};

#define STR_PARAM(namePar, len, txt)struct namePar\
{\
	typedef Holder<len> type_value;\
	type_value value;\
	type_value default_value;\
	const wchar_t *name(){return L###namePar;}\
	namePar()\
	{\
		default_value = (wchar_t *)txt;\
		value = (wchar_t *)default_value;\
	}\
};

#define CHAR_PARAM(namePar, len, txt)struct namePar\
{\
	typedef CharHolder<len> type_value;\
	type_value value;\
	type_value default_value;\
	const wchar_t *name(){return L###namePar;}\
	namePar()\
    {\
	   default_value = txt;\
	   value = default_value;\
    }\
};
	//L#Wrap##L#z;
#define DEFINE_PARAM_WRAP(Wrap, z, type, def_val) template<> struct Wrap<z>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#Wrap###z;}\
	Wrap() : value(def_val), default_value(def_val) {}\
};

#define DEFINE_PARAM_WRAP2(Wrap0, Wrap1, z, type, def_val) template<> struct Wrap0<Wrap1<z> >\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#Wrap0###Wrap1###z;}\
	Wrap0() : value(def_val), default_value(def_val) {}\
};

#define DEFINE_PARAM_Wrap_NUM(Wrap, z, n, type, def_val) template<> struct Wrap<z, n>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#Wrap##L#z##L#n;}\
	Wrap() : value(def_val), default_value(def_val) {}\
};

#define DEFINE_ARRAY_PARAM_WRAP(Wrap, z, type, count, def_val) template<>struct Wrap<z>\
{\
	typedef type type_value[count];\
	type_value value;\
	const type default_value;\
	const wchar_t *name(){return L#Wrap##L#z;}\
	Wrap(): default_value(def_val) \
    {\
		for(int i = 0; i < count; ++i) value[i] = def_val;\
	}\
};

#define DEFINE_ARRAY_PARAM_NO_INIT(z, type, count) struct z\
{\
	typedef type type_value[count];\
	type_value value;\
	const wchar_t *name(){return L#z;}\
};

#define DEFINE_WRAP(Name, type, def_val)template<>struct Name	 \
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	Name() : value(def_val), default_value(def_val)\
    {\
	 TrimTypeList(Buf(),L#Name);\
    }\
	static wchar_t *Buf(){static wchar_t buf[dimention_of(L#Name)]; return buf;};\
	wchar_t *name(){return Buf();}\
};

#define DEFINE_PARAM_WAPPER(wapper, z, type, def_val) template<> struct wapper<z>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#wapper L#z;}\
	wapper() : value(def_val), default_value(def_val) {}\
};



