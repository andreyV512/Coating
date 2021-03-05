#include <stdio.h>
#include "App/App.h"
#include "Common/templates/impl.hpp"
#include "Compute/InitFiltre.hpp"
#include "Compute/SetTresholds.hpp"

int main()
{
	Impl<IDSPFlt, 1032> filter[App::count_sensors];
	FiltersTable params;
	params.items.get<Num<CurrentFilter, 0>>().value = 1;
	__wrap_filters__ x(filter, 250, params.items);
	__init_filtre__()(x);
	IDSPFlt *f = filter[0];

	dprint("jgug %s\n", typeid(filter[0]).name());

	for (int i = 0; i < 10; ++i)
	{
		double t = (*f)(i);
		printf("%d %f\n", i, t);
	}
}

