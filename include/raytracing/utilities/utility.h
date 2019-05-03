#pragma once

namespace rt
{

template<class T>
struct DeletePointerFunctor
{
	void operator()(const T* pT) const
	{
		delete pT, pT = NULL;
	}
};

}
