#pragma once
#include "Graph.h"
template <class Graph> class Comparer;

template<typename T>
class Comparer
{
public:
	bool operator()(T* n1, T* n2)
	{
		return n2->m_data.m_heuristic + n2->m_data.m_localDistance < n1->m_data.m_heuristic + n1->m_data.m_localDistance;
	}
};