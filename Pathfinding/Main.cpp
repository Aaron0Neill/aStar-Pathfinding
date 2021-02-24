//author: @Aaron O Neill

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "Graph.h"
#include "NodeData.h"
#include "VisualPath.h"

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

void visit(Node* t_node)
{
	std::cout << "Visiting: " << t_node->m_data.m_name << " Local: " << t_node->m_data.m_localDistance << " Global: " << t_node->m_data.m_heuristic << std::endl;
};

int main()
{
	Graph<NodeData, int>graph(30);

	NodeData nodeData;
	nodeData.m_obstacle = false;
	int nodeIndex = 0;
	std::ifstream nodeInfo;

	std::unordered_map<std::string, int> nodeMap;

	nodeInfo.open("nodes.txt");
	while (nodeInfo >> nodeData.m_name >> nodeData.m_location.first >>  nodeData.m_location.second)
	{
		nodeMap[nodeData.m_name] = nodeIndex;
		graph.addNode(nodeData, nodeIndex++);
	}
	nodeInfo.close();

	nodeInfo.open("arcs.txt");
	std::string from, to;
	int weight;
	while (nodeInfo >> from >> to >> weight)
	{
		graph.addArc(nodeMap.at(from), nodeMap.at(to), weight);
	}
	nodeInfo.close();

	VisualPath m_visalise(graph, visit);

	m_visalise.Run();

	std::cin.get();
	system("Pause");

}