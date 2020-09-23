#include <stdio.h>
#include "DspFilters/Filters.hpp"
#include "DspFilters/DspFiltrParams.h"

int main()
{
	printf("%s\n", typeid(Conv<type_flites_list>::Result).name());
	
	VL::Factory<filters_list> filters;
	
	FiltersTable params;
	params.items.get<CurrentFilter>().value = 5;

	IDSPFlt *proc = NULL;
	__init_filtre_data__ data(proc, params.items);
	VL::foreach<filters_list, __init_filtre__>()(filters, data);

	for (int i = 0; i < 100; ++i)
	{
		double t = (*proc)(i);
		printf("%d %f\n", i, t);
	}
}

