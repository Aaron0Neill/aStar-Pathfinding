#include "VisualPath.h"


VisualPath::VisualPath(Graph<NodeData, int>& t_data, std::function<void(Node*)> t_visit) :
	m_window{ sf::VideoMode{ 1000U, 800U, 32U }, "Visual Test" },
	m_data(t_data),
	f_visit(t_visit),
	m_start(9),
	m_end(18),
	m_radius(20.0f),
	m_connections(sf::Lines),
	m_finalPath(sf::Lines),
	m_notDiscovered(sf::Color(255U,0U,0U)),
	m_discovered(sf::Color(50U,50U,150U)),
	m_pathColor(sf::Color(100U,100U,255U))
{
	if (!m_font.loadFromFile("assets/arial.ttf"))
	{
		std::cout << "ERROR LOADING FONT\n";
	}

	m_infoText.setFont(m_font);
	m_infoText.setString("Searched Nodes - Dark Blue\nUnsearched Nodes - Red\nStart Node - Green\nEnd Node - Yellow\nPath -Light Blue");
	m_infoText.setPosition(530, 0);

	for (int i = 0; i < 30; ++i)
	{
		m_nodes[i].setRadius(m_radius);
		float x = static_cast<float>(t_data.nodeIndex(i)->m_data.m_location.first);
		float y = static_cast<float>(t_data.nodeIndex(i)->m_data.m_location.second);
		m_nodes[i].setPosition(x, y);
		m_nodeName[i].setFont(m_font);
		m_nodeName[i].setString(t_data.nodeIndex(i)->m_data.m_name);
		m_nodeName[i].setPosition(x + (m_radius/2.0f), y);
		m_nodeName[i].setFillColor(sf::Color::Black);
	}
	ShowLinks();
	m_data.aStar(m_data.nodeIndex(m_start), m_data.nodeIndex(m_end), f_visit, m_path); // go from '1' to 'v'
	updatePath();
	ShowPath();
}

//*********************************************

void VisualPath::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);
	while (m_window.isOpen())
	{
		HandleInputs();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			HandleInputs();
			Update();
		}
		Render();
	}
}

//*********************************************

void VisualPath::Render()
{
	m_window.clear();

	m_window.draw(m_connections);
	m_window.draw(m_finalPath);

	for (auto node : m_nodes)
	{
		m_window.draw(node);
	}

	for (auto text : m_nodeName)
	{
		m_window.draw(text);
	}
	m_window.draw(m_infoText);

	m_window.display();
}

//*********************************************

void VisualPath::Update()
{
}

//*********************************************

void VisualPath::HandleInputs()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (newEvent.type == sf::Event::MouseButtonPressed)
		{
			if (newEvent.mouseButton.button == sf::Mouse::Button::Left)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(m_window);

				for (int i = 0; i < 30; ++i)
				{
					if (m_nodes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(pos)))
					{
						m_data.nodeIndex(i)->m_data.m_obstacle = !m_data.nodeIndex(i)->m_data.m_obstacle;
						m_path.clear();
						m_data.aStar(m_data.nodeIndex(m_start), m_data.nodeIndex(m_end), f_visit, m_path); // go from '1' to 'v'
						updatePath();
						ShowPath();
					}
				}
			}
		}
		if (newEvent.type == sf::Event::KeyPressed)
		{
			if (newEvent.key.code == sf::Keyboard::LControl)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(m_window);

				for (int i = 0; i < 30; ++i)
				{
					if (m_nodes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(pos)))
					{
						m_end = i;
						m_path.clear();
						m_data.aStar(m_data.nodeIndex(m_start), m_data.nodeIndex(m_end), f_visit, m_path); // go from '1' to 'v'
						updatePath();
						ShowPath();
					}
				}
			}
			else if (newEvent.key.code == sf::Keyboard::LShift)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(m_window);

				for (int i = 0; i < 30; ++i)
				{
					if (m_nodes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(pos)))
					{
						m_start = i;
						m_path.clear();
						m_data.aStar(m_data.nodeIndex(m_start), m_data.nodeIndex(m_end), f_visit, m_path); // go from '1' to 'v'
						updatePath();
						ShowPath();
					}
				}
			}
		}
	}
}

//*********************************************

void VisualPath::updatePath()
{
	for (int i = 0; i < 30; ++i)
	{
		m_nodes[i].setFillColor((m_data.nodeIndex(i)->marked()) ? m_discovered : m_notDiscovered);
		if (m_data.nodeIndex(i)->m_data.m_obstacle)
			m_nodes[i].setFillColor(sf::Color(100U, 100U, 100U, 255U));
		else if (i == m_start)
			m_nodes[i].setFillColor(sf::Color::Green);
		else if (i == m_end)
			m_nodes[i].setFillColor(sf::Color::Yellow);
		float x = static_cast<float>(m_data.nodeIndex(i)->m_data.m_location.first);
		float y = static_cast<float>(m_data.nodeIndex(i)->m_data.m_location.second);
		m_nodes[i].setPosition(x, y);
	}
}

//*********************************************

void VisualPath::ShowLinks()
{
	std::queue<Node*> links;

	links.push(m_data.nodeIndex(0));

	while (!links.empty())
	{
		links.front()->setMarked(true);
		sf::Vertex current = sf::Vertex(sf::Vector2f{ links.front()->m_data.m_location.first + (m_radius / 2.0f),
		links.front()->m_data.m_location.second + (m_radius / 2.0f) }, sf::Color::Blue);
		for (auto node : links.front()->arcList())
		{
			if (!node.node()->marked())
			{
				sf::Vertex end = sf::Vertex(sf::Vector2f{ node.node()->m_data.m_location.first + (m_radius / 2.0f),
					node.node()->m_data.m_location.second + (m_radius / 2.0f) }, sf::Color::Blue);
				m_connections.append(current);
				m_connections.append(end);
				links.push(node.node());
			}
		}
		links.pop();
	}
}

//*********************************************

void VisualPath::ShowPath()
{
	m_finalPath.clear();
	for (int i = 0; i < m_path.size() - 1; ++i)
	{
		sf::Vertex current = sf::Vertex(sf::Vector2f{ m_path.at(i)->m_data.m_location.first + (m_radius / 2.0f),
		m_path.at(i)->m_data.m_location.second + (m_radius / 2.0f) }, sf::Color::Yellow);
		sf::Vertex next = sf::Vertex(sf::Vector2f{ m_path.at(i + 1)->m_data.m_location.first + (m_radius / 2.0f),
		m_path.at(i + 1)->m_data.m_location.second + (m_radius / 2.0f) }, sf::Color::Yellow);

		m_finalPath.append(current);
		m_finalPath.append(next);
	}

	for (int i = 0; i < 30; ++i)
	{
		for (int j = 1; j < m_path.size() - 1; ++j)
		{
			if (m_data.nodeIndex(i) == m_path.at(j))
			{
				m_nodes.at(i).setFillColor(m_pathColor);
			}
		}
	}
}
