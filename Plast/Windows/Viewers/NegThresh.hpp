#pragma once

struct __set_thresh_data__
{
	unsigned alThr_color;
	double   alThr_value;
	double      alThr_startOffs;
	double      alThr_stopOffs;
	unsigned btmRefThr_color;
	double   btmRefThr_value;
	double      btmRefThr_startOffs;
	double      btmRefThr_stopOffs;
};

template<class O, class P>struct __set_thresh__
{
	void operator()(O &o, P &p)
	{
		auto &alThr = o.tchart.items.get<AlThr>();
		auto &btmRefThr = o.tchart.items.get<BtmRefThr>();

		alThr.color = p.alThr_color;
		alThr.value = p.alThr_value;
		alThr.startOffs = p.alThr_startOffs;
		alThr.stopOffs = p.alThr_stopOffs;
		btmRefThr.color = p.btmRefThr_color;
		btmRefThr.value = p.btmRefThr_value;
		btmRefThr.startOffs = p.btmRefThr_startOffs;
		btmRefThr.stopOffs = p.btmRefThr_stopOffs;
	}
};

template<class TreshItems, class Viewers>void SetTresh(TreshItems &treshItems, Viewers &viewers)
{
	auto &color = Singleton<ColorTable>::Instance().items;

	//TODO XXX__set_thresh_data__ data = {
	//TODO XXX   color.get< Clr<Defect>>().value
	//TODO XXX   , treshItems.get<AlarmThresh>().value
	//TODO XXX   , treshItems.get<AlarmThreshStart>().value
	//TODO XXX   , treshItems.get<AlarmThreshStop>().value
	//TODO XXX   , color.get< Clr<NoBottomReflection>>().value
	//TODO XXX   , treshItems.get<BottomReflectionThresh>().value
	//TODO XXX   , treshItems.get<BottomReflectionThreshStart>().value
	//TODO XXX   , treshItems.get<BottomReflectionThreshStop>().value
	//TODO XXX};
	//TODO XXXVL::foreach<typename VL::Inner<Viewers>::Result, __set_thresh__>()(viewers, data);
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