#include "CPI.h"
extern map<int, GNode>GgraphNode;
extern map<int, GEdge>GgraphEdge;
extern map<int, qNode>QgraphNode;
extern map<int, qEdge>QgraphEdge;
map <int, set<int>> newUC;
//map<int, vector<int>> TopCPI;
map<int, map<int, map<int,set<int>>>> N;//N[k1][k2][k3]means k1 ->k2 of k3 has set<int>N[k1][k2][k3]; 
map<int, set<int>> level;
map<int, set<int>> uNebor;
using namespace std;
void TopDown(){
	//root
	int r = getRoot();
	//cout << "R" << r << endl;
	int Cnt = 0;
	int up=r;
	//storage u.UN 
	set<int> un;
	set<int> uc;
	set<int> t3;
	map<int, set<int>> t;
	//set<map<int, set<int>>>t4;
	map<int,map<int, set<int>>> t2;
	vector<int> a;
	/*a.push_back(1);
	
	t.insert(pair<int, vector<int>>(2, a));
	N.insert(pair<int, map<int, vector<int>>>(0, t));
	cout << "hh" << N[0][2][0];*/
	//viste the root
	
	// from 2 to max level
	BFS_Level();
	for (map<int, GNode>::iterator it = GgraphNode.begin(); it != GgraphNode.end(); ++it)
	{
		if (it->second.label == QgraphNode[r].label&&it->second.degree >= QgraphNode[r].degree)
		{
			if (CandVerify(it->first, r))
			{
				uc.insert(it->first);
			}
		}
	}
	//TopCPI.insert(pair<int,vector<int>>(r,a));
	newUC.insert(pair<int, set<int>>(r, uc));
	QgraphNode[r].visted = true;
	//a.clear();
	//for (map<int, qNode>::iterator it = QgraphNode.begin(); it != QgraphNode.end(); ++it) { it->second.visted = false; }
	for (map<int, set<int>>::iterator it = ++level.begin(); it != level.end(); ++it) //it means each level lev
	{
		//Forward Candiata Generation
		for (set<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) // it2 means each query vertex u at level lev
		{
			uc.clear();
			Cnt = 0;
			un.clear();
			for (set<int>::iterator it3 = QgraphEdge[*it2].Edges.begin(); it3 != QgraphEdge[*it2].Edges.end(); ++it3)// it3 means  each query vertex u' of Nq(u)
			{
				if (!QgraphNode[*it3].visted&&(it->second.find(*it3) != it->second.end())) 
				{
					//cout << *it3 << "ii" << endl;
					un.insert(*it3);
					uNebor.insert(pair<int, set<int>>(*it2, un));
				}
				else if (QgraphNode[*it3].visted) 
				{
					//cout << *it3 << "qq" << endl;
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
			//
			for (map<int, GNode>::iterator it6 = GgraphNode.begin(); it6 != GgraphNode.end(); ++it6) 
			{
				if (it6->second.cnt == Cnt) 
				{
					if (CandVerify(it6->first, *it2)) 
					{
						uc.insert(it6->first);
					}

				}
			}
			newUC.insert(pair<int, set<int>>(*it2, uc));
			QgraphNode[*it2].visted = true;
			for (map<int, GNode>::iterator it6 = GgraphNode.begin(); it6 != GgraphNode.end(); ++it6) { it6->second.cnt = 0; }

		}
		// Backward Candiate Pruning 
		for (set<int>::iterator it2 = --(it->second.end()); it2 != --(it->second.begin());--it2) // it2 means each query vertex u at level lev in reverse order
		{
			Cnt = 0;
			if(uNebor[*it2].size()>0){
			for (set<int>::iterator it3 = uNebor[*it2].begin(); it3 != uNebor[*it2].end(); ++it3)// it3 means each query vertex u' of u.UN
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
			for (set<int>::iterator it6 = newUC[*it2].begin(); it6 != newUC[*it2].end(); ++it6)//it3 means each vertex of u.C
			{
				//cout << *it6 << "yy" << endl;
				if (GgraphNode[*it6].cnt != Cnt) 
				{
					a.push_back(*it6);
				}
			} 
			for(vector<int>::iterator i=a.begin();i!=a.end();i++)
			{
				newUC[*it2].erase(*i);
			}
			a.clear();
			for (map<int, GNode>::iterator it6 = GgraphNode.begin(); it6 != GgraphNode.end(); ++it6) { it6->second.cnt = 0; }
		}
		}
		
		// Adjacency List Construction
		for (set<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) // it2 means each query vertex u at level lev in reverse order
		{			
			up = QgraphNode[*it2].p;			
			//t4.clear();
			t.clear();
			t2.clear();
			for (set<int>::iterator it3 = newUC[up].begin(); it3 != newUC[up].end(); ++it3)//it3 means each vertex vp of up.C
			{
							
				t3.clear();
				for(set<int>::iterator it4=GgraphEdge[*it3].Edges.begin();it4 !=GgraphEdge[*it3].Edges.end();++it4)//it4 means each vertex v of NG(vp)
				{
					//newUC[*it2].f;
					if(newUC[*it2].find(*it4)!= newUC[*it2].end())
					{
						t3.insert(*it4);
						
					}
				}
				t.insert(pair<int, set<int>>(*it3, t3));
				
				
			}
			//t4.insert(t);
			t2.insert(pair<int, map<int, set<int>>>(*it2, t));
			if (N.find(up) == N.end()) {
				N.insert(pair<int, map<int, map<int, set<int>>>>(up, t2));
			}
			else 
			{
				N[up].insert(t2.begin(),t2.end());
			}

		}
	
	}
	// reset all vertex visted is false
	for (map<int, qNode>::iterator it = QgraphNode.begin(); it != QgraphNode.end(); ++it)
	{
		it->second.visted = false;
	}
} 
void BFS_Level() 
{
	set<int> Temp; 
	set<int> Temp2;
	//get root
	int r = getRoot();
	int hight = 0;
	Temp2.insert(r);
	level.insert(pair<int, set<int>>(hight++, Temp2));
	QgraphNode[r].visted = true;	
	Temp.insert(r);
	//s
	while (Temp.size() > 0) {
		Temp2.clear();
		//level.insert(pair<int, set<int>>(hight++, Temp));
		for (set<int>::iterator it = Temp.begin(); it != Temp.end(); ++it)//it means parent vertex u.p
		{
			for (set<int>::iterator it2 = QgraphEdge[*it].Edges.begin(); it2 != QgraphEdge[*it].Edges.end(); ++it2)// it2 means the child u of u.p
			{
				if (!QgraphNode[*it2].visted)
				{
					Temp2.insert(*it2);
					// if (QgraphNode[*it2].p != -1) {
						QgraphNode[*it2].p = *it;
					//}
					QgraphNode[*it2].visted = true;
				}
			}
		}
		if (Temp2.size() > 0) {
			level.insert(pair<int, set<int>>(hight++, Temp2));
		}
		else
			break;
		Temp.clear();
		Temp = Temp2;
	}
	// reset all vertex visted is false
	for (map<int, qNode>::iterator it = QgraphNode.begin(); it != QgraphNode.end(); ++it) 
	{
		it->second.visted = false;
	}
}
void cleanCPI() 
{
	newUC.clear();
	N.clear();
	level.clear();
	uNebor.clear();
}