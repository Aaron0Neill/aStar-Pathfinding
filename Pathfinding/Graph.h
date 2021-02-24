#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <vector>
#include <functional>
#include <stack>
#include "Comparer.h"

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

// ---------------------------------------------------------------------
//  Name:           Graph
//  Description:    Manages the nodes and connections (arc) between them
// ---------------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph 
{
public:        
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node * nodeIndex(int index) const 
	{
		return m_nodes.at(index);
    }

    // Public member functions.
    bool addNode( NodeType data, int index );
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
    void depthFirst( Node* node, std::function<void(Node *)> f_visit);
	
    void breadthFirst( Node* node, std::function<void(Node *)> f_visit);
	void adaptedBreadthFirst( Node* current, Node* goal );	
	void ucs(Node* start, Node* dest, std::function<void(Node*)> f_visit, std::vector<Node*>& path);
    void aStar(Node* start, Node* dest, std::function<void(Node*)> f_visit, std::vector<Node*>& path);

private:

	// ----------------------------------------------------------------
	//  Description:    A container of all the nodes in the graph.
	// ----------------------------------------------------------------

	std::vector<Node*> m_nodes;

};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int maxNodes ) : m_nodes( maxNodes, nullptr) 
{  
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   for( int index = 0; index < m_nodes.size(); index++ ) 
   {
        if( m_nodes[index] != nullptr ) 
		{
			delete m_nodes.at(index);
        }
   }
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode( NodeType data, int index ) 
{
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( nullptr == m_nodes.at(index) ) 
   {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
	  m_nodes.at(index) = new Node;
	  m_nodes.at(index)->m_data = data;
	  m_nodes.at(index)->setMarked(false);
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) 
{
     // Only proceed if node does exist.
     if( nullptr != m_nodes.at(index) ) 
	 {
         // now find every arc that points to the node that
         // is being removed and remove it.        
         Arc* arc;

         // loop through every node
         for( int node = 0; node < m_nodes.size(); node++ ) 
		 {
              // if the node is valid...
              if( nullptr != m_nodes.at(node) ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_nodes.at(node)->getArc(m_nodes.at(index) );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_nodes.at(index);
		m_nodes.at(index) = nullptr;       
    }
}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) 
{
     bool proceed = true; 
     // make sure both nodes exist.
     if( nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to) ) 
	 {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_nodes.at(from)->getArc( m_nodes.at(to) ) != nullptr ) 
	 {
         proceed = false;
     }

     if (proceed == true) 
	 {
        // add the arc to the "from" node.
		 m_nodes.at(from)->addArc(m_nodes.at(to), weight );
     }
        
     return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) 
{
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to) )
	 {
         nodeExists = false;
     }

     if (nodeExists == true) 
	 {
        // remove the arc.
		 m_nodes.at(from)->removeArc(m_nodes.at(to) );
     }
}

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) 
{
     Arc* arc = 0;
     // make sure the to and from nodes exist
     if( nullptr != m_nodes.at(from) && nullptr != m_nodes.at(to) ) 
	 {
         arc = m_nodes.at(from)->getArc(m_nodes.at(to) );
     }
                
     return arc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() 
{
     for( int index = 0; index < m_nodes.size(); index++ ) 
	 {
          if( nullptr != m_nodes.at(index) ) 
		  {
			  m_nodes.at(index)->setMarked(false);
          }
     }
}


// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* node, std::function<void(Node *)> f_visit ) 
{
     if( nullptr != node ) {
           // process the current node and mark it
           f_visit( node );
           node->setMarked(true);

           // go through each connecting node
           auto iter = node->arcList().begin();
           auto endIter = node->arcList().end();
        
		   for( ; iter != endIter; ++iter) 
		   {
			    // process the linked node if it isn't already marked.
                if ( (*iter).node()->marked() == false ) 
				{
                   depthFirst( (*iter).node(), f_visit);
                }            
           }
     }
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* node, std::function<void(Node *)> f_visit) 
{
   if( nullptr != node ) 
   {
	  std::queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( node );
      node->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) 
	  {
         // process the node at the front of the queue.
		 f_visit( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         auto iter = nodeQueue.front()->arcList().begin();
         auto endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) 
		 {
              if ( (*iter).node()->marked() == false) 
			  {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}


// ----------------------------------------------------------------
//  Name:           adaptedBreadthFirst
//  Description:    Performs a breadth-first traversal the starting node
//                  specified as an input parameter, terminating at the goal.
//  Arguments:      The first parameter is the starting node.
//                  The second parameter is the goal node.
//  Return Value:   None.
// ----------------------------------------------------------------
// * Design and logic was made with the help of:
// Ben Millar - https://github.com/ben-millar *
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::adaptedBreadthFirst( Node* current, Node *goal ) 
{
    if (nullptr != current && nullptr != goal)
    {
        std::queue<Node*> nodes;

        nodes.push(current);
        current->setMarked(true);

        while (!nodes.empty())
        {
            for (auto arcs : nodes.front()->arcList())
            { // loop through all the lists 
                if (arcs.node()->marked() == false)
                { // only check unmarked nodes
                    nodes.push(arcs.node());
                    nodes.back()->setMarked(true);
                    nodes.back()->setPrevious(nodes.front());

                    if (goal == nodes.back())
                    {//early exit 
                        return;
                    }
                }
            }
            nodes.pop();
        }
    }
}

// ----------------------------------------------------------------
//  Name:           UCS
//  Description:    Searches through the graph in order to find a path
//  Arguments:      The first is where the search should start
//					The second is the node you are looking for
//					The third is a function pointer to the output what node has been visited
//					The fourth is the vector that will record the path between the vectors
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::ucs(Node* start, Node* dest, std::function<void(Node*)> f_visit, std::vector<Node*>& path)
{
	for (auto node : m_nodes)
	{ // reset the heuristic for all the nodes
		node->m_data.m_localDistance = std::numeric_limits<int>::max();
	}

	std::priority_queue < Node*, std::vector<Node*>, Comparer<Node>> priorQueue; // create the priority queue using a comparer class

	start->setPrevious(nullptr); //set the start to point to null so there is a clear stop point
	start->m_data.m_localDistance = 0; // set the heuristic as its the start

	priorQueue.push(start);
	start->setMarked(true);

	while (!priorQueue.empty())
	{ // constantly loop until the destination is found or all nodes have been checked
		for (auto arc : priorQueue.top()->arcList())
		{
			if (!arc.node()->marked())
			{
				arc.node()->setMarked(true);
				arc.node()->setPrevious(priorQueue.top());
				arc.node()->m_data.m_localDistance = priorQueue.top()->m_data.m_localDistance + arc.weight();
				priorQueue.push(arc.node());
			}
			else if(arc.node()->m_data.m_localDistance > priorQueue.top()->m_data.m_localDistance + arc.weight())
			{
				arc.node()->setPrevious(priorQueue.top());
				arc.node()->m_data.m_localDistance = priorQueue.top()->m_data.m_localDistance + arc.weight();
			}
			
			if (dest == priorQueue.top())
			{
				Node* current = priorQueue.top();
				while (current != nullptr)
				{
					path.push_back(current);
					current = current->previous();
				}
				return;
			}
		}
		priorQueue.pop();
	}
}

// ----------------------------------------------------------------
//  Name:           A Star
//  Description:    Searches through the graph in order to find the shortest path between two points
//  Arguments:      The first is where the search should start
//					The second is the node you are looking for
//					The third is a function pointer to the output what node has been visited
//					The fourth is the vector that will record the path between the vectors
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::aStar(Node* start, Node* dest, std::function<void(Node*)> f_visit, std::vector<Node*>& path)
{
    //lambda distance
    auto getDistanceToGoal = [&](Node* current, Node* goal) {    return sqrt(pow(goal->m_data.m_location.first - current->m_data.m_location.first, 2) + pow(goal->m_data.m_location.second - current->m_data.m_location.second, 2)); };

    // reset all the nodes local and global goals
    for (auto node : m_nodes)
    {
        node->m_data.m_localDistance = std::numeric_limits<int>::max();
        node->m_data.m_heuristic = getDistanceToGoal(node, dest);//update the global distance
        node->setMarked(false);
        node->setPrevious(nullptr);
    }

    std::priority_queue<Node*, std::vector<Node*>, Comparer<Node>> priorQueue;

    //initial setup
    start->setMarked(true);
    start->m_data.m_localDistance = 0;

    priorQueue.push(start);

    while (!priorQueue.empty() && dest != priorQueue.top())
    {
        Node* current = priorQueue.top();
        priorQueue.pop();

        f_visit(current);

        for (auto arc : current->arcList())
        {
            if (!arc.node()->m_data.m_obstacle)
            { // do nothing if the node is an obstacle
                float distance = current->m_data.m_localDistance + arc.weight(); // get the local distance 
                if (distance < arc.node()->m_data.m_localDistance) 
                { // update the parent node and local distance if its shorter than the current local
                    arc.node()->setPrevious(current); // set the parent 
                    arc.node()->m_data.m_localDistance = distance;
                }

                if (!arc.node()->marked())
                { // if the node hasnt been marked
                    arc.node()->setMarked(true); // mark the node
                    priorQueue.push(arc.node()); // add it to the list
                }
            }
        }
    }
    Node* current = dest;
    std::stack<Node*> invertPath;
    while (current != nullptr)
    {
        invertPath.push(current);
        current = current->previous();
    }
    while (!invertPath.empty())
    {
        path.push_back(invertPath.top());
        invertPath.pop();
    }
}

#include "GraphNode.h"
#include "GraphArc.h"


#endif
