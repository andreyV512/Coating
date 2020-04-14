#pragma once
template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};





