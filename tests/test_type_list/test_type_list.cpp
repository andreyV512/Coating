#include <stdio.h>
#include <typeinfo>
#include "templates/typelist.hpp"
//H:\projects\Test\Coating\CommonApp\MessageText\status.h
//H:\projects\Test\Coating\tests\test_type_list\test_type_list.cpp
#include "H:\projects\Test\Coating\CommonApp\MessageText\status.h"
#include "H:\projects\Test\Coating\CommonApp\MessageText\status.hpp"





int main()
{
	//int num = 0;
	//VL::foreach<ZoneStatus::status_list, __print__>()(num);

	 //////////////////////////////
	//ZoneStatus::SkipList - ok
	//ZoneStatus::СrossLists - ok
	///////////////////////////////////////


	//Status st;
	//int t = st(1, 2);
	//printf("result %d\n", t);

	//st.Test();
	StatusData::Test();
   //typedef Vlst<noBottomReflection, defect, SensorOff, deadZone, Norm > zone_status_list;
	//ZoneStatus::SkipList<noBottomReflection, __skip_list__>::Result skipList;
	//printf("%s\n", typeid(skipList).name());
	bool b = ZoneStatus::Skip<Vlst<Norm, noBottomReflection>>::value;
		
	printf("%d\n", b);
}
