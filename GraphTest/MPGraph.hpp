#pragma once

#include <map>
#include <memory>
#include <unordered_map>

#include "MPGraphNode.hpp"
#include "MPGraphIterator.hpp"

using std::tr1::shared_ptr;

	template<typename Tn, typename Tk>
	class MPGraphIterator;

template<typename Tpayload, typename Tkey>
class MPGraph
{
public:
	MPGraph(void);
	~MPGraph(void);

	bool AddNode(const Tpayload & node, const Tkey id);
	bool AddAndLinkNodes(Tpayload node1, Tkey id1, Tpayload node2, Tkey id2);
	bool LinkNodes(Tkey idNode1, Tkey idNode2);

	bool GetNode(const Tkey id, Tpayload * result);
	void GetNodeConnections(const Tkey nodeID, std::list<Tpayload*> & connections);
	void GetNodeConnectionsFirstNLevels(const Tkey nodeID, std::vector<std::list<Tpayload> > & connections, const size_t numberOfLevels);


	typedef MPGraphIterator<Tpayload, Tkey> Iterator;

	typename Iterator Begin();
	typename Iterator End();

private:
	std::map<Tkey, shared_ptr<MPGraphNode<Tpayload, Tkey> > > _nodes;

};

template<typename Tpayload, typename Tkey>
MPGraph<Tpayload, Tkey>::MPGraph(void)
{
}


template<typename Tpayload, typename Tkey>
MPGraph<Tpayload, Tkey>::~MPGraph(void)
{
}


template<typename Tpayload, typename Tkey>
bool MPGraph<Tpayload, Tkey>::AddNode(const Tpayload & node, const Tkey id)
{
	std::map<Tkey, shared_ptr<MPGraphNode<Tpayload, Tkey> > > :: iterator item = _nodes.find(id);
	if(item != _nodes.end())
		return false;

	//shared_ptr<MPGraphNode<Tpayload, Tkey> > node_ptr(new MPGraphNode<Tpayload, Tkey>(node));
	_nodes[id] = shared_ptr<MPGraphNode<Tpayload, Tkey> > (new MPGraphNode<Tpayload, Tkey>(node, id));

	return true;
}

template<typename Tpayload, typename Tkey>
bool MPGraph<Tpayload, Tkey>::AddAndLinkNodes(Tpayload node1, Tkey id1, Tpayload node2, Tkey id2)
{
	AddNode(node1, id1);
	AddNode(node2, id2);
	if(LinkNodes(id1, id2))
		return true;
	return false;	
}

template<typename Tpayload, typename Tkey>
bool MPGraph<Tpayload, Tkey>::LinkNodes(Tkey idNode1, Tkey idNode2)
{
	std::map<Tkey, shared_ptr<MPGraphNode<Tpayload, Tkey> > > :: iterator item1 = _nodes.find(idNode1);
	if(item1 == _nodes.end())
		return false;

	std::map<Tkey, shared_ptr<MPGraphNode<Tpayload, Tkey> > > :: iterator item2 = _nodes.find(idNode2);
	if(item2 == _nodes.end())
		return false;

	(*item1).second->AddConnection((*item2).second.get());
	(*item2).second->AddConnection((*item1).second.get());

	return true;
}

template<typename Tpayload, typename Tkey>
bool MPGraph<Tpayload, Tkey>::GetNode(const Tkey id, Tpayload * result)
{
	std::map<Tkey, shared_ptr<MPGraphNode<Tpayload, Tkey> > > :: iterator item = _nodes.find(id);
	if(item == _nodes.end())
		return false;

	(*item).second->GetPayload(result);
	return true;
}

template<typename Tpayload, typename Tkey>
void MPGraph<Tpayload, Tkey>::GetNodeConnections(const Tkey nodeID, std::list<Tpayload*> & connections)
{

}

template<typename Tpayload, typename Tkey>
typename MPGraphIterator<Tpayload, Tkey> MPGraph<Tpayload, Tkey>::Begin()
{
	if(_nodes.empty())
		return Iterator();

	shared_ptr<MPGraphNode<Tpayload, Tkey> > firstNode = _nodes.begin()->second;
	Iterator gi(firstNode.get());
	return gi;
}

template<typename Tpayload, typename Tkey>
typename MPGraphIterator<Tpayload, Tkey> MPGraph<Tpayload, Tkey>::End()
{
	Iterator gi;
	return gi;
}

template<typename Tpayload, typename Tkey>
void MPGraph<Tpayload, Tkey>::GetNodeConnectionsFirstNLevels(const Tkey nodeID, std::vector<std::list<Tpayload> > & connections, const size_t numberOfLevels)
{
	Iterator nodeIter = this->Begin();
	Iterator end = this->End();
	int currentLevel = 0;

	while(nodeIter != end && currentLevel < numberOfLevels)
	{
		if(connections.size() < currentLevel + 1)
			connections.push_back(std::list<Tpayload>());

		connections[currentLevel].push_back(*nodeIter);
		if(nodeIter.IsLastNodeOnLevel())
		{
			++currentLevel;
		}

		++nodeIter;
	}
}

