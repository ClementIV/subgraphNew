#include "CFL.h"
set<int> core;
set<int> forest;
set<int> leaf;
extern map<int, GNode>GgraphNode;
extern map<int, GEdge>GgraphEdge;
extern map<int, qNode>QgraphNode;
extern map<int, qEdge>QgraphEdge;
//map<int, set<int>>CoreTree;
map<int, set<int>>ForestTree;
map<int, set<int>>LeafTree;
map<int, set<int>>CoreGraph;
//map<int, GNode>GgraphNode2;
//map<int, GEdge>GgraphEdge2;
//map<int, qNode>QgraphNode2;
map<int, qEdge>QgraphEdge2;
void copy() 
{
	for (map<int, qEdge>::iterator it = QgraphEdge.begin(); it != QgraphEdge.end(); ++it)
	{
		QgraphEdge2.insert(pair<int,qEdge>(it->first, it->second));
	}
}

void CFL_Descompose() 
{
	copy();
	int Tcount = 1;
	int temp = 0;
	bool isEnd = false;
	set<int> Edges;
	while (!isEnd) {
		isEnd = true;
		for (map<int, qEdge>::iterator it2 = QgraphEdge2.begin(); it2 != QgraphEdge2.end(); ++it2)// it2 means each u of q
		{
				
			if (it2->second.Edges.size() == 1)//it2 has one degree
			{
				isEnd = false;
				if (Tcount <= 1) {
					leaf.insert(it2->first);
				//Tcount++;
				}
				else if(Tcount>=2)
				{
					forest.insert(it2->first);
					
				}
				// find the node id;
				temp = *(it2->second.Edges.begin());//temp means the nebor u' of u 
				//cout << it2->first <<"L or F"<< endl;
				//find the node above and erase it2->first(the edge)
				Edges.clear();
				if (Tcount == 1) 
				{
					Edges.insert(it2->first);
					if (LeafTree.find(temp) != LeafTree.end())
					{
						LeafTree[temp].insert(Edges.begin(),Edges.end());
					}
					else {
						LeafTree.insert(pair<int, set<int>>(temp, Edges));
					}
				}
				else 
				{
					Edges.insert(it2->first);
					if (ForestTree.find(temp) != ForestTree.end())
					{
						ForestTree[temp].insert(Edges.begin(), Edges.end());
					}
					else {
						ForestTree.insert(pair<int, set<int>>(temp, Edges));
					}
				}
				Edges.clear();
				QgraphEdge2[temp].Edges.erase(QgraphEdge2[temp].Edges.find(it2->first));// u'remove nebor u
				it2->second.Edges.clear();
				
			}
					
			if (it2->second.Edges.size() == 0) 
			{
				if (forest.find(it2->first) == forest.end() && leaf.find(it2->first) == leaf.end())
				{
					core.insert(it2->first);
					CoreGraph.insert(pair<int, set<int>>(it2->first, it2->second.Edges));
				}
			}
		}
		Tcount++;
		if (!isEnd) { continue; }
		else {
			for (map<int, qEdge>::iterator it2 = QgraphEdge2.begin(); it2 != QgraphEdge2.end(); ++it2)//it2 means each u of q in core
			{

				if (it2->second.Edges.size() > 1)
				{
					//cout << it2->first;
					CoreGraph.insert(pair<int, set<int>>(it2->first, it2->second.Edges));
					core.insert(it2->first);
				}
			}
		}
	}
	
}
void cleanCFL() 
{
	core.clear();
	forest.clear();
	leaf.clear();
	ForestTree.clear();
	LeafTree.clear();
	CoreGraph.clear();
	QgraphEdge2.clear();
}