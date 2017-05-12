#include "CandVerify.h"
#include "CPI.h"
extern map<int, GNode>GgraphNode;
extern map<int, GEdge>GgraphEdge;
extern map<int, qNode>QgraphNode;
extern map<int, qEdge>QgraphEdge;
extern set<int> core;
extern set<int> forest;
extern set<int> leaf;
map <int,set<int>> UC;
extern vector<int> order;
extern map<int, map<int, int>>MCore;;
int getRoot() 
{
	// step1. use a light-weight strategy to obtain C(u) based on only the label lq(u) and dq(u) 
	set<int> C;
	vector<int> a;
	int i = 0;
	for (set<int>::iterator it = core.begin(); it != core.end(); ++it)
	{
		C.clear();
		for(map<int,GNode>::iterator it2 = GgraphNode.begin(); it2 != GgraphNode.end(); ++it2)
			if (QgraphNode[*it].label == it2->second.label&&it2->second.degree >= QgraphNode[*it].degree) 
			{
				C.insert(it2->first);
			}
		UC.insert(pair<int,set<int>>(*it,C));
	}
	//step2. Top-3 vertices use CandVerify
	for (map<int, set<int>>::iterator it = UC.begin(); it != UC.end();it++) // it means each C(u)
	{
		a.clear();
		i = 0;
		for (set<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) 
		{
			if (!CandVerify(*it2, it->first)) 
			{
				a.push_back(*it2);
			}
			/*i++;
			if (i % 10 == 0) { cout << i << endl; }*/
		}
		for (vector<int>::iterator itx = a.begin(); itx != a.end(); ++itx) 
		{
			it->second.erase(it->second.find(*itx));
		}
	}
	//step3. find the smallest |C(u)|/dq(u) as root;
	int root = UC.begin()->first;
	double min=0;
	if (QgraphNode[UC.begin()->first].degree > 0) {
		min = UC.begin()->second.size() / QgraphNode[UC.begin()->first].degree;
		for (map<int, set<int>>::iterator it = UC.begin(); it != UC.end(); it++)
		{
			if ((it->second.size() / QgraphNode[it->first].degree) < min)
			{
				min = (it->second.size() / QgraphNode[it->first].degree);
				root = it->first;
			}
		}
	}	
	// return root
	return root;
}
bool CandVerify(int v, int u) 
{
	if (mndG(v) < mndq(u))
	{
		return false;
	}
	for (set<int>::iterator it = QgraphEdge[u].Edges.begin(); it != QgraphEdge[u].Edges.end(); ++it) 
	{
		//QgraphNode[u].label;
		if (dG(v, QgraphNode[u].label) < dq(u, QgraphNode[u].label))
			return false;
	}
	return true;
} 
// return the v's maximum neighbor-degree
int mndG( int v) 
{
	set<int>::iterator it = GgraphEdge[v].Edges.begin();
	int max = 0;
	//cout << *it << " ";
	if (it != GgraphEdge[v].Edges.end())
	{
		 max= GgraphNode[*it].degree;
	}
	else 
	{
		return 0;
	}
	//cout << max << endl;
	for (;it != GgraphEdge[v].Edges.end();++it) 
	{
		if (GgraphNode[*it].degree > max) 
		{
			max = GgraphNode[*it].degree;
		}
	}
	return max;
}
int mndq( int u)
{
	set<int>::iterator it = QgraphEdge[u].Edges.begin();
	int max = 0;
	if (it != QgraphEdge[u].Edges.end()) {
	 max= QgraphNode[*it].degree;
	}
	else { return 0; }
	for (; it != QgraphEdge[u].Edges.end(); ++it)
	{
		if (QgraphNode[*it].degree > max)
		{
			max = QgraphNode[*it].degree;
		}
	}
	return max;
}
int dG(int v, string l)
{
	int result = 0;
	for (set<int>::iterator it = GgraphEdge[v].Edges.begin(); it != GgraphEdge[v].Edges.begin(); ++it) 
	{
		if (GgraphNode[*it].label == l) 
		{
			result++;
		}
	}
	return result;
}
int dq(int u, string l)
{
	int result = 0;
	for (set<int>::iterator it = QgraphEdge[u].Edges.begin(); it != QgraphEdge[u].Edges.begin(); ++it)
	{
		if (QgraphNode[*it].label == l)
		{
			result++;
		}
	}
	return result;
}
bool ValidateNT(int v, int i,map<int,int>M)
{
	int j = 0;
	for (set<int>::iterator it = QgraphEdge[order[i]].Edges.begin(); it != QgraphEdge[order[i]].Edges.end(); ++it) //it means each nebor uj of ui
	{
		for (j = 0; j < order.size(); j++)
		{
			if (order[j] == *it)
			{
				break;
			}
		}
		if ( j< i&&QgraphNode[order[i]].p != *it)
		{
				if (GgraphEdge[v].Edges.find(M[order[j]]) == GgraphEdge[v].Edges.end())
				{
					return false;
				}
			
		}
	}
	return true;
}
void cleanCand() { UC.clear(); }