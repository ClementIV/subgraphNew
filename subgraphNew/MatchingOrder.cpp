#include "CPI.h"
extern map<int, GNode>GgraphNode;
extern map<int, GEdge>GgraphEdge;
extern map<int, qNode>QgraphNode;
extern map<int, qEdge>QgraphEdge;
//extern map<int, qEdge>QgraphEdge2;
extern map <int, set<int>> newUC;
//map<int, vector<int>> TopCPI;
extern map<int, map<int, map<int, set<int>>>> N;
// pai and order 没必要一直在这，可以改写成临时变量；
extern map<int, set<int>> level;
map<int, RTL> pai;
map<int, vector<int>> Forestpai;
map<int, vector<int>> Leafpai;
extern set<int> core;
//extern set<int> forest;
//extern set<int> leaf;
map<int, set<int>>CoreTree;
extern map<int, set<int>>ForestTree;
extern map<int, set<int>>LeafTree;
extern map<int, set<int>>CoreGraph;
map<int, int>CoreNT;
vector<int> order;
vector<int> Forder;
vector<int> Lorder;
// embedding 
map<int,map<int, int>>MCore;
map<int, map<int, int>>MForest;
map<int, map<int, int>>MLeaf;
//map<int, int>MC;
static int  numC = 1;
static int numF = 1;
static int numL = 1;
static int coreNum = 0;
static int ForestNum = 0;
static int LeafNum = 0;
void MatchOrderCore() 
{
	GetcoreTree();
	RTL rtl;
	rtl.NT = 0;
	int r = getRoot();
	int Num = 0;
	//int tempRoot;
	// reset all vertex visted is false
	/*for (map<int, qNode>::iterator it = QgraphNode.begin(); it != QgraphNode.end(); ++it)
	{
		it->second.visted = false;
	}*/
	// mask r is visited
	QgraphNode[r].visted = true;
	bool isEnd = false;
	map<int, int>Cpi;
	//find pai and NT
	// find RTL and insert into pai
	//use getCorePai() to 
	/*set<int>::iterator temp = QgraphEdge[r].Edges.begin();	
	//while (temp!= QgraphEdge[r].Edges.end()) {
	//	++temp;
	//	rtl.vertex.clear();
	//	rtl.NT = 0;
	//	rtl.vertex.push_back(r);
	//	
	//	for (map<int, set<int>>::iterator it = ++level.begin(); it != level.end(); ++it) //it means each level lev
	//	{
	//		for (set<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) //it2 means each u of same lev
	//		{
	//			if (core.find(*it2) == core.end()) 
	//			{
	//				isEnd = true;
	//				break;
	//			}
	//			else if (!QgraphNode[*it2].visted)
	//			{
	//				rtl.vertex.push_back(*it2);
	//				QgraphNode[*it2].visted = true;
	//				for (set<int>::iterator it3 = QgraphEdge[*it2].Edges.begin(); it3 != QgraphEdge[*it2].Edges.end(); ++it3) //it3 means each nebor of u
	//				{
	//					if (it->second.find(*it3) != it->second.end())
	//					{
	//						rtl.NT++;
	//					}
	//					else{ 
	//						--it;
	//						if (it->second.find(*it3) != it->second.end() && QgraphNode[*it2].p != *it3)
	//						{
	//							rtl.NT++;
	//						}
	//						++it;
	//					}
	//				}
	//				break;
	//			}
	//		}
	//		if (isEnd)break;
	//	}
	//	if (!isEnd) {
	//		pai.insert(pair<int, RTL>(Num++, rtl));
	//	}
	//}
	*/
	getCorePi(rtl,r);
	for (map<int, RTL>::iterator it = pai.begin(); it != pai.end(); ++it) 
	{
		Cpi.insert(pair<int, int>(it->first, C(it->second.vertex)));
	}
	double minPaiNum;
	if (pai.begin()->second.NT != 0) {
		 minPaiNum = Cpi.begin()->second / pai.begin()->second.NT;
	}
	else {  minPaiNum = Cpi.begin()->second; }
	double tempPaiNum = minPaiNum;
	RTL minPai = pai.begin()->second;
	set<int> orderHas;	
	while (pai.size() > 0) {
		Num = pai.begin()->first;
		minPai = pai.begin()->second;
		for (map<int, RTL>::iterator it = pai.begin(); it != pai.end(); ++it)
		{	
			/*cout << "pai--->" <<pai.size()<< it->first<<":";
			for (vector<int>::iterator itx = it->second.vertex.begin(); itx != it->second.vertex.end(); ++itx) 
			{
				cout << *itx << " ";
			}
			cout << "| " << it->second.NT<<endl;*/
			if (it->second.NT !=0) {
				tempPaiNum = (double)Cpi[it->first] / it->second.NT;
			}
			else { tempPaiNum = (double)Cpi[it->first]; }
			if (tempPaiNum < minPaiNum)
			{
				minPai = it->second;
				minPaiNum = tempPaiNum;
				Num = it->first;
			}
		}		
		for (vector<int>::iterator it = minPai.vertex.begin(); it != minPai.vertex.end(); ++it) 
		{
			if (orderHas.find(*it) == orderHas.end()) {
				order.push_back(*it);
			}
			orderHas.insert(*it);
		};
		
		pai.erase(pai.find(Num));
		
	}
}
int C(vector<int> pi) 
{
	int sum = 0;
	int t1,t2;
	map<int, map<int, int>> uVCount;//C[u][v]
	map<int, int> Vcount;//C(v)
	vector<int>::iterator it = --pi.end();
	for (set<int>::iterator it2 = newUC[*it].begin(); it2 != newUC[*it].end(); ++it2) {// it means each u of a pai
		Vcount.insert(pair<int, int>(*it2, 1));
	}
	uVCount.insert(pair<int, map<int, int>>(*it, Vcount));
	for (;it != pi.begin(); ) //it means each u of a pai ? 是否要第一个
	{
		t1 = *it;//means u'
		t2 = *(--it);//means u
		Vcount.clear();
		for (set<int>:: iterator it2=newUC[t2].begin();it2!=newUC[t2].end();++it2) //it2 means each v of u.C
		{
			
			sum = 0;
			for (set<int>::iterator it3 = N[t2][t1][*it2].begin(); it3 != N[t2][t1][*it2].end(); ++it3)//it3 means each vertex v' that in u' of v that in u
			{
				sum += uVCount[t1][*it3];
			}
			Vcount.insert(pair<int, int>(*it2, sum));
		}
		uVCount.insert(pair<int, map<int, int>>(t2,Vcount));
	}
	sum = 0;
	for (map<int, int>::iterator it2 = uVCount.begin()->second.begin(); it2 != uVCount.begin()->second.end(); ++it2)//it2 means each v of r.C
	{
		sum =sum+it2->second;
	}
	return sum;
}
void CoreMatch(int i, map<int, int>MC)
{
	/*cout << i << ":: ";
	for (map<int, int>::iterator it2 = MC.begin(); it2 != MC.end(); ++it2)
	{
		cout << it2->first << "->" << it2->second << " ";
	}
	cout << endl;*/
	
	//set<int> t;
	if (i == core.size()) {
		MCore.insert(pair<int, map<int, int>>(numC++, MC));
		return;
	}
	else if (i == 0) 
	{
		
		for (set<int>::iterator it = newUC[order[i]].begin(); it != newUC[order[i]].end(); ++it)//it means each v of ui.C
		{
			MC.clear();
			//t.insert(*it);
			MC.insert(pair<int,int>(order[i],*it));
			//cout << *it << "qqqqqq" << endl;
			GgraphNode[*it].visted = true ;
			CoreMatch(i + 1,MC);
			GgraphNode[*it].visted = false;
		}
	}
	else 
	{
		//for (map<int,int>::iterator it = MC[QgraphNode[order[i]].p].begin(); it != MC[QgraphNode[order[i]].p].end(); ++it) // MC[QgraphNode[order[i]].p] means v of M(ui.p)
		
			for (set<int>::iterator it2 = N[QgraphNode[order[i]].p][order[i]][MC[QgraphNode[order[i]].p]].begin(); it2 != N[QgraphNode[order[i]].p][order[i]][MC[QgraphNode[order[i]].p]].end(); ++it2)//it2 means each v of v.p in ui
			{
				//cout << QgraphNode[order[i]].p << "pp" << endl;
				if ((!(GgraphNode[*it2].visted))&&ValidateNT(*it2,i,MC))
				{
					//cout << *it2 << "c" << endl;
					//t.insert(*it);
					MC.erase(order[i]);
					MC.insert(pair<int, int>(order[i], *it2));
					GgraphNode[*it2].visted = true;
					CoreMatch(i + 1,MC);
					GgraphNode[*it2].visted = false;
				}
			}
		
	}
}
void GetcoreTree() //ragarding CoreTree ,ForestTree and Leaf Tree and coreNT 
{
	int t;
	set<int> children;
	map<int, set<int>>::iterator t1;
	map<int, set<int>>::iterator t2;
	map<int, bool> CoreVisited;
	for (set<int>::iterator it = core.begin(); it != core.end(); ++it)// it means each u of core 
	{
		CoreNT.insert(pair<int, int>(*it,0));
		CoreVisited.insert(pair<int,bool>(*it, false));
	}
	for (map<int, set<int>>::iterator it = level.begin(); it != level.end(); ) //it means each level lev
	{
		t1 = it;
		t2 = ++it;
		if (t2 != level.end()) {
			t = t2->first;
			for (set<int>::iterator it2 = t1->second.begin(); it2 != t1->second.end(); ++it2)//it2 means each u in same lev
			{
				children.clear();
				if (core.find(*it2) != core.end())
				{
					CoreVisited[*it2] = true;
					for (set<int>::iterator it3 = CoreGraph[*it2].begin(); it3 != CoreGraph[*it2].end(); ++it3) //it3 means each child u' of u
					{
						
							if (level[t].find(*it3) != level[t].end()&&!CoreVisited[*it3])//u' is a real child of u
							{
								children.insert(*it3);
								
							}
							else if (QgraphNode[*it2].p!=*it3)// E(u',u) is a Non-tree and not u.p->u
							{
								CoreNT[*it2] += 1;
							}
							CoreVisited[*it3] = true;
					}
				}
				CoreTree.insert(pair<int, set<int>>(*it2, children));
			}
		}
		else 
		{
			for (set<int>::iterator it2 = t1->second.begin(); it2 != t1->second.end(); ++it2) //it2 means each u in same lev in last level
			{
				if (core.find(*it2) != core.end())
				{
					CoreVisited[*it2] = true;
					for (set<int>::iterator it3 = CoreGraph[*it2].begin(); it3 != CoreGraph[*it2].end(); ++it3) //it3 means each child u' of u
					{
						if (QgraphNode[*it2].p != *it3)// E(u',u) is a Non-tree and not u.p->u
						{
							CoreNT[*it2] += 1;//u's non-tree ++;
						}
						//CoreVisited[*it3] = true;
					}
				}
			}
		}
	}
}
void getCorePi(RTL rtl,int r) 
{
	if (CoreTree[r].size() == 0) 
	{
		rtl.vertex.push_back(r);
		rtl.NT += CoreNT[r];
		pai.insert(pair<int,RTL>(coreNum++, rtl));
		return;
	}
	else
	{
		for (set<int>::iterator it = CoreTree[r].begin(); it != CoreTree[r].end(); ++it)//it means each child u of r
		{
			rtl.vertex.push_back(r);
			rtl.NT += CoreNT[r];
			getCorePi(rtl, *it);
			rtl.vertex.pop_back ();
			rtl.NT -= CoreNT[r];
		}
	}

}
void getForestPi(vector<int>vertex, int r)
{
	if (ForestTree[r].size() == 0)
	{
		vertex.push_back(r);
		Forestpai.insert(pair<int, vector<int>>(ForestNum++, vertex));
		return;
	}
	else
	{
		for (set<int>::iterator it =ForestTree[r].begin(); it != ForestTree[r].end(); ++it)//it means each child u of r
		{
			vertex.push_back(r);			
			getForestPi(vertex, *it);
			vertex.pop_back();			
		}
	}
}
void getLeafPi(vector<int> vertex, int r) 
{
	if (LeafTree[r].size() == 0)
	{
		vertex.push_back(r);
		Leafpai.insert(pair<int, vector<int>>(LeafNum++, vertex));
		return;
	}
	else
	{
		for (set<int>::iterator it = LeafTree[r].begin(); it != LeafTree[r].end(); ++it)//it means each child u of r
		{
			vertex.push_back(r);
			getLeafPi(vertex, *it);
			vertex.pop_back();
		}
	}
}
void MatchOrderForest()
{
	vector<int> vertex;
	map<int, int>Fpi;
	vector<int> minpi;
	double tempMinpi=0;
	double minPiNum = 0;
	int Num=0;
	set<int> Forderhas;
	// get all FoerstPai
	for (map<int, set<int>>::iterator it = ForestTree.begin(); it != ForestTree.end(); ++it) //it->first means each u in forest
	{
		if (core.find(it->first) != core.end())
		{
			vertex.clear();
			getForestPi(vertex, it->first);

		}
	}
	// get all Fpi;
	for (map<int, vector<int>>::iterator it = Forestpai.begin(); it != Forestpai.end(); ++it) //it means each pai of forest
	{
		Fpi.insert(pair<int, int>(it->first, C(it->second)));
	}
	//order by increasing Fpi
	while (Forestpai.size() > 0) {
		minpi = Forestpai.begin()->second;
		Num = Forestpai.begin()->first;
		minPiNum = Fpi[Forestpai.begin()->first];
		for (map<int, vector<int>>::iterator it = Forestpai.begin(); it != Forestpai.end(); ++it)
		{
			tempMinpi = Fpi[it->first];
			if (tempMinpi < minPiNum)
			{
				minPiNum = tempMinpi;
				Num = it->first;
			}
		}
		for (vector<int>::iterator it = Forestpai[Num].begin(); it != Forestpai[Num].end(); ++it)//it means each u of samllest Fpi
		{
			if (Forderhas.find(*it) == Forderhas.end())
			{
				Forder.push_back(*it);
			}
			Forderhas.insert(*it);
		}
		Forestpai.erase(Forestpai.find(Num));
	}
}
void MatchOrderLeaf()
{
	vector<int> vertex;
	map<int, int>Lpi;
	vector<int> minpi ;
	double tempMinpi = 0;
	double minPiNum = 0;
	int Num = 0;
	set<int> Lorderhas;
	// get all FoerstPai
	for (map<int, set<int>>::iterator it = LeafTree.begin(); it != LeafTree.end(); ++it) //it->first means each u in forest
	{
		if (ForestTree.find(it->first) != ForestTree.end()||CoreTree.find(it->first)!=CoreTree.end())
		{
			vertex.clear();
			getLeafPi(vertex, it->first);

		}
	}
	// get all Fpi;
	for (map<int, vector<int>>::iterator it = Leafpai.begin(); it !=Leafpai.end(); ++it) //it means each pai of forest
	{
		Lpi.insert(pair<int, int>(it->first, C(it->second)));
	}
	//order by increasing Fpi
	while (Leafpai.size() > 0) {
		minpi = Leafpai.begin()->second;
		Num = Leafpai.begin()->first;
		minPiNum = Lpi[Leafpai.begin()->first];
		for (map<int, vector<int>>::iterator it = Leafpai.begin(); it != Leafpai.end(); ++it)
		{
			tempMinpi = Lpi[it->first];
			if (tempMinpi < minPiNum)
			{
				minPiNum = tempMinpi;
				Num = it->first;
			}
		}
		for (vector<int>::iterator it = Leafpai[Num].begin(); it != Leafpai[Num].end(); ++it)//it means each u of samllest Fpi
		{
			if (Lorderhas.find(*it) == Lorderhas.end())
			{
				Lorder.push_back(*it);
			}
			Lorderhas.insert(*it);
		}
		Leafpai.erase(Leafpai.find(Num));
	}
}
void ForestMatch(int i, map<int, int>MF, map<int, int>MC) 
{
	//set<int> t;
	if (i == ForestTree.size()) {
		for (map<int, int>::iterator it = MC.begin(); it != MC.end(); ++it)// it means each u in core that mapped v
		{
			if (MF.find(it->first) == MF.end()) 
			{
				MF.insert(pair<int,int>(it->first,it->second));
			}
		}
		MForest.insert(pair<int, map<int, int>>(numF++, MF));
		return;
	}
	
	else if (core.find(Forder[i])!=core.end())// u is a core node ,u has mapped so
	{
		if (i == 0) {
			MF.clear();
		}
			//MF.clear();
			//t.insert(*it);
		MF.insert(pair<int, int>(Forder[i], MC[Forder[i]]));
			//cout << *it << "qqqqqq" << endl;
		GgraphNode[MC[Forder[i]]].visted = true;
		ForestMatch(i + 1, MF,MC);
		GgraphNode[MC[Forder[i]]].visted = false;
		
	}
	else
	{
		//for (map<int,int>::iterator it = MC[QgraphNode[order[i]].p].begin(); it != MC[QgraphNode[order[i]].p].end(); ++it) // MC[QgraphNode[order[i]].p] means v of M(ui.p)

		for (set<int>::iterator it2 = N[QgraphNode[Forder[i]].p][Forder[i]][MF[QgraphNode[Forder[i]].p]].begin(); it2 != N[QgraphNode[Forder[i]].p][Forder[i]][MF[QgraphNode[Forder[i]].p]].end(); ++it2)//it2 means each v of v.p in ui
		{
			//cout << QgraphNode[order[i]].p << "pp" << endl;
			if ((!(GgraphNode[*it2].visted)))
			{
				//cout << *it2 << "c" << endl;
				//t.insert(*it);
				MF.erase(Forder[i]);
				MF.insert(pair<int, int>(Forder[i], *it2));
				GgraphNode[*it2].visted = true;
				ForestMatch(i + 1, MF,MC);
				GgraphNode[*it2].visted = false;
			}
		}

	}
}
void LeafMatch(int i, map<int, int>ML, map<int, int>MF) 
{
	if (i == LeafTree.size()) {
		for (map<int, int>::iterator it = MF.begin(); it != MF.end(); ++it)// it means each u in Forest or core that mapped v
		{
			if (ML.find(it->first) == ML.end())
			{
				ML.insert(pair<int, int>(it->first, it->second));
			}
		}
		MLeaf.insert(pair<int, map<int, int>>(numL++, ML));
		return;
	}

	else if (core.find(Lorder[i]) != core.end()||ForestTree.find(Lorder[i])!=ForestTree.end())// u is a core node ,u has mapped so
	{
		if (i == 0) {
			ML.clear();
		}
		//MF.clear();
		//t.insert(*it);
		ML.insert(pair<int, int>(Lorder[i], MF[Lorder[i]]));
		//cout << *it << "qqqqqq" << endl;
		GgraphNode[MF[Lorder[i]]].visted = true;
		LeafMatch(i + 1, ML, MF);
		GgraphNode[MF[Lorder[i]]].visted = false;

	}
	else
	{
		//for (map<int,int>::iterator it = MC[QgraphNode[order[i]].p].begin(); it != MC[QgraphNode[order[i]].p].end(); ++it) // MC[QgraphNode[order[i]].p] means v of M(ui.p)

		for (set<int>::iterator it2 = N[QgraphNode[Lorder[i]].p][Lorder[i]][MF[QgraphNode[Lorder[i]].p]].begin(); it2 != N[QgraphNode[Lorder[i]].p][Lorder[i]][MF[QgraphNode[Lorder[i]].p]].end(); ++it2)//it2 means each v of v.p in ui
		{
			//cout << QgraphNode[order[i]].p << "pp" << endl;
			if ((!(GgraphNode[*it2].visted)))
			{
				//cout << *it2 << "c" << endl;
				//t.insert(*it);
				MF.erase(Lorder[i]);
				MF.insert(pair<int, int>(Lorder[i], *it2));
				GgraphNode[*it2].visted = true;
				LeafMatch(i + 1, ML,MF);
				GgraphNode[*it2].visted = false;
			}
		}

	}
}
void MatchAll() 
{
	
	MatchOrderCore();
	MatchOrderForest();
	MatchOrderLeaf();
	map<int, int>MC;
	map<int, int>MF;
	map<int, int>ML;
	CoreMatch(0,MC);
	for (map<int, map<int, int>>::iterator it = MCore.begin(); it != MCore.end(); ++it) // it means each M of core
	{
		ForestMatch(0,MF,it->second);
	}

	for (map<int, map<int, int>>::iterator it =MForest.begin(); it != MForest.end(); ++it) // it means each M of core
	{
		LeafMatch(0, ML, it->second);
	}



}
void cleanMatch() {
	pai.clear();
	Forestpai.clear();
	Leafpai.clear();
	CoreTree.clear();
	CoreNT.clear();
	 order.clear();
	 Forder.clear();
	 Lorder.clear();
	// embedding 
	MCore.clear();
	MForest.clear();
	MLeaf.clear();
}
