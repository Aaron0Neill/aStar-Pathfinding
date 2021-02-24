#ifndef VISUAL_PATH_INCLUDE
#define VISUAL_PATH_INCLUDE

#include "Graph.h"
#include "NodeData.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

typedef GraphArc <NodeData, int > Arc;
typedef GraphNode<NodeData, int> Node;

class VisualPath
{
public:
	VisualPath(Graph<NodeData, int>& t_data, std::function<void(Node*)> f_visit);
	~VisualPath() = default;

	void Run();

	void Render();
	void Update();
	void HandleInputs();

	void updatePath();

	void ShowLinks();

	void ShowPath();

private:
	std::array<sf::CircleShape, 30> m_nodes;
	std::array<sf::Text, 30> m_nodeName;
	sf::Font m_font;
	sf::Text m_infoText;
	sf::RenderWindow m_window;
	Graph<NodeData, int>& m_data;
	std::vector<Node*> m_path;
	std::function<void(Node*)> f_visit;
	int m_start;
	int m_end;
	float m_radius;
	sf::VertexArray m_connections;
	sf::VertexArray m_finalPath;
	sf::Color m_notDiscovered;
	sf::Color m_discovered;
	sf::Color m_pathColor;
};

#endif