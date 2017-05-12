#include "CPI.h"
extern map<int, GNode>GgraphNode;
extern map<int, GEdge>GgraphEdge;
extern map<int, qNode>QgraphNode;
extern map<int, qEdge>QgraphEdge;
extern map <int, set<int>> newUC;
//map<int, vector<int>> TopCPI;
extern map<int, map<int, map<int, set<int>>>> N;
extern map<int, set<int>> level;
void BottomUp() 
{
	TopDown();
	int Cnt = 0;
	vector<int> a;
	for (map<int, set<int>>::iterator it = --level.end(); it != --level.begin(); --it) // it means each level lev
	{
		 
		for (set<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) //it2 means each u of same lev
		{
			//cout << *it2 << "sssss" << endl;
			Cnt = 0;
			// Candiate Refinement
			// same as TopDown
			for (set<int>::iterator it3 = QgraphEdge[*it2].Edges.begin(); it3 != QgraphEdge[*it2].Edges.end(); ++it3) 
			{
				if (QgraphNode[*it3].visted) 
				{
					for (set<int>::iterator it4 = newUC[*it3].begin(); it4 != newUC[*it3].end(); ++it4) // it4 means each vertex v' of u'.C
					{
						for (set<int>::iterator it5 = GgraphEdge[*it4].Edges.begin(); it5 != GgraphEdge[*it4].Edges.end(); ++it5) // it5 means each vertex v that of NG(v')
						{
							if ((GgraphNode[*it5].label == QgraphNode[*it2].label) && GgraphNode[*it5].degree >= QgraphNode[*it2].degree)
							{
								if (GgraphNode[*it5].cnt == Cnt)
								{
									GgraphNode[*it5].cnt++;
								}
							}
						}

					}
					Cnt++;
				}
			}
			//remove 
			a.clear();
			for (set<int>::iterator it3 = newUC[*it2].begin(); it3 != newUC[*it2].end(); ++it3) 
			{
				if (GgraphNode[*it3].cnt != Cnt) 
				{
					a.push_back(*it3);
				}
			}
			for(vector<int>::iterator it3=a.begin();it3!=a.end();++it3)
			{
				newUC[*it2].erase(*it3);
				for (map<int, map<int, set<int>>>::iterator it4 = N[*it2].begin(); it4 != N[*it2].end(); ++it4) 
				{
					it4->second.erase(*it3);
				}
				
			}
			
			//reset all v.cnt=0;
			for (map<int, GNode>::iterator it6 = GgraphNode.begin(); it6 != GgraphNode.end(); ++it6) { it6->second.cnt = 0; }			
			// Adjancy List Pruning
			for (set<int>::iterator it3 = newUC[*it2].begin(); it3 != newUC[*it2].end(); ++it3) // it3 means each vertex v of u.C
			{
				for (set<int>::iterator it4 = QgraphEdge[*it2].Edges.begin(); it4 != QgraphEdge[*it2].Edges.end(); ++it4) //it4 means each child u' of u
				{
					if(QgraphNode[*it4].p==*it2 && it->second.find(*it4)==it->second.end())//check if it4 is a real bfsTree child
					{
						a.clear();
						for (set<int>::iterator it5 = N[*it2][*it4][*it3].begin(); it5 != N[*it2][*it4][*it3].end(); ++it5)//it5 means each v' of that nebors of v between u and u' 
						{
							if (newUC[*it4].find(*it5) == newUC[*it4].end()) // v' not in u'.C
							{
								//N[u][u'][v] remove v' 
								//cout << *it5 << "tt" << endl;
								a.push_back(*it5);
							}
						}
						for (vector<int>::iterator it6 = a.begin(); it6 != a.end(); ++it6)
						{
							N[*it2][*it4][*it3].erase(*it6);
						}
				
					}
				}
			}
			
		}
		for (set<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			QgraphNode[*it2].visted = true;
		}
	}
}