#pragma once

template<class O, class P>struct __set_thresh__
{
	void operator()(O &o, P &p)
	{		
		auto &alThr = o.tchart.items.get<AlThr>();
		auto &btmRefThr = o.tchart.items.get<BtmRefThr>();
		auto &color = Singleton<ColorTable>::Instance().items;

		alThr.color = color.get< Clr<Defect>>().value;
		alThr.value = p.get<Num<AlarmThresh, P::NUM>>().value;
		alThr.startOffs = p.get<Num<AlarmThreshStart, P::NUM>>().value;
		alThr.stopOffs = p.get<Num<AlarmThreshStop, P::NUM>>().value;
		btmRefThr.color = color.get< Clr<NoBottomReflection>>().value;
		btmRefThr.value = p.get<Num<BottomReflectionThresh, P::NUM>>().value;
		btmRefThr.startOffs = p.get<Num<BottomReflectionThreshStart, P::NUM>>().value;
		btmRefThr.stopOffs = p.get<Num<BottomReflectionThreshStop, P::NUM>>().value;
	}
};

template<template<int>class Sens, int NUM, class P>struct __set_thresh__<Sens<NUM>, P>
{
	typedef Sens<NUM> O;
	void operator()(O &o, P &p)
	{
		auto &alThr = o.tchart.items.get<AlThr>();
		auto &btmRefThr = o.tchart.items.get<BtmRefThr>();
		auto &color = Singleton<ColorTable>::Instance().items;

		alThr.color = color.get< Clr<Defect>>().value;
		alThr.value = p.get<Num<AlarmThresh, NUM>>().value;
		alThr.startOffs = p.get<Num<AlarmThreshStart, NUM>>().value;
		alThr.stopOffs = p.get<Num<AlarmThreshStop, NUM>>().value;
		btmRefThr.color = color.get< Clr<NoBottomReflection>>().value;
		btmRefThr.value = p.get<Num<BottomReflectionThresh, NUM>>().value;
		btmRefThr.startOffs = p.get<Num<BottomReflectionThreshStart,NUM>>().value;
		btmRefThr.stopOffs = p.get<Num<BottomReflectionThreshStop, NUM>>().value;
	}
};

template<class TreshItems, class Viewers>void SetTresh(TreshItems &treshItems, Viewers &viewers)
{
	VL::foreach<typename VL::Inner<Viewers>::Result, __set_thresh__>()(viewers, treshItems);
}

template<class A, class B>void __assignment_param__(A &a, B &b)
{
	a.value = b.value;
	a.startOffs = b.startOffs;
	a.stopOffs = b.stopOffs;
	a.color = b.color;
	a.widthPen = b.widthPen;
	a.dashStyle = b.dashStyle;
}

template<class O, class P>struct __bipolar__
{
	void operator()(O &o, P &p)
	{
		__assignment_param__(o, p.get<typename VL::Inner<O>::Result>());
		o.value *= -1;
	}
};
template<class O, class P>struct __monopolar__
{
	void operator()(O &o)
	{
		o.value = 0;
	}
};

template<class O, class P>struct __switch_bipolar__
{
	void operator()(O &o, P &p)
	{
		typedef VL::SelectWrap<typename O::TChart::items_list, Neg>::Result list;
		if (p)
		{
			o.tchart.minAxesY = -100;
			VL::foreach<list, __bipolar__>()(o.tchart.items, o.tchart.items);
		}
		else
		{
			o.tchart.minAxesY = 0;
			VL::foreach<list, __monopolar__>()(o.tchart.items);
		}
	}
};