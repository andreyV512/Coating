#pragma once

class __init_filtre__
{
	template<class O, class P>struct __init_filtre_XX__
	{
		bool operator()(P &p)
		{
			if (VL::IndexOf<filters_list, O>::value == p.paramFlt.get<Num<CurrentFilter, P::NUM>>().value)
			{
				p.filter[P::NUM].Init<O>();
				SetupFiltre<O, P::NUM>()(
					(O &)p.filter
					, p.paramFlt
					, p.frequency
					);
				return false;
			}
			return true;
		}
	}; 

	template<class O, class P>struct loc
	{
		void operator()(P &t)
		{
			static const int i = O::value;
			if (VL::find<filters_list, __init_filtre_XX__>()(xz_type<i>(t))) t.filter[i].Init<DSPFltDump>();
		}
	};
public:
	template<class T>void operator()(T &t)
	{
		VL::foreach<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, loc>()(t);
	}
};
