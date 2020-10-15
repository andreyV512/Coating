#pragma once
#include "templates/typelist.hpp"
#include "Graphics/Borders.h"

template<class T>class Neg : public HOffsBorder
{
public:
	Neg(Chart &c) : HOffsBorder(c) {}
	void Draw() { if (this->value < 0)HOffsBorder::Draw(); }
};

class AlThr : public HOffsBorder { public: AlThr(Chart &c) : HOffsBorder(c) {} };
class BtmRefThr : public HOffsBorder { public: BtmRefThr(Chart &c) : HOffsBorder(c) {} };