#pragma once
#include  <string>

// ---------------------------------------------------------------------
//  Name:           NodeData
//  Description:    A simple struct to encapsulate all the node data.
//                  Add or remove fields as required.               
// ---------------------------------------------------------------------
struct NodeData
{
	bool m_obstacle;
	std::string m_name;
	int m_localDistance; // local goal 
	int m_heuristic; // global goal
	std::pair<int, int> m_location; // x and y position of the node
};