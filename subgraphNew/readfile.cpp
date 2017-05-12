
#include "readfile.h"
 map<int, GNode>GgraphNode;
 map<int, GEdge>GgraphEdge;
 map<int, qNode>QgraphNode;
 map<int, qEdge>QgraphEdge;
using namespace std;

void getData(string GN,string GE,string qN,string qE ) 
{
	
	
	//read files
	ifstream G_Node(GN, ios::in);
	ifstream G_Edge(GE,ios::in);
	ifstream q_Node(qN, ios::in);
	ifstream q_Edge(qE, ios::in);
	// stroage
	//set<int>::iterator it;
	set<int> Ggraph;
	
	/*vector<vector<GNode>> query;*/
	//templete value
	 int m;
	 int n;
	 string l;
	 GNode G;
	 GEdge E;
	 //read
	 //G_Node >> l;
	 // get the G_Node
	while (G_Node>>m) 
	{	
		getline(G_Node,l);
		G.label = l;
		G.degree = 0;
		G.cnt = 0;
		G.visted = false;
		GgraphNode.insert(pair<int,GNode>(m,G));
		GgraphEdge.insert(pair<int, GEdge>(m,E));
		
	}
	//cout << GgraphNode.find(2)->second.label;
	// get the G_Edge
	//G_Edge >> l;
	while (G_Edge>>m>>n) 
	{ 
		//E.Edges.insert(n);
		if (GgraphEdge[m].Edges.find(n) == GgraphEdge[m].Edges.end()) {
			GgraphEdge[m].Edges.insert(n);
			GgraphEdge[n].Edges.insert(m);
			GgraphNode[m].degree += 1;
			GgraphNode[n].degree += 1;
		}
	}
	qNode Q;
	qEdge EQ;
	// get the q nodes
	//q_Node >> l;
	while (q_Node >> m)
	{
		getline(q_Node, l);
		Q.label = l;
		Q.degree = 0;
		Q.cnt = 0;
		Q.visted = false;
		Q.p = -1;
		QgraphNode.insert(pair<int, qNode>(m, Q));
		QgraphEdge.insert(pair<int, qEdge>(m, EQ));

	}
	//cout << GgraphNode.find(2)->second.label;
	//get q edges
	//q_Edge >> l;
	while (q_Edge >> m >> n)
	{
		//E.Edges.insert(n);
		if (QgraphEdge[m].Edges.find(n) == QgraphEdge[m].Edges.end()) {
			QgraphEdge[m].Edges.insert(n);
			QgraphEdge[n].Edges.insert(m);
			QgraphNode[m].degree += 1;
			QgraphNode[n].degree += 1;
		}
	}
}
void print() 
{
	for (map<int, qEdge>::iterator it2 = QgraphEdge.begin(); it2 != QgraphEdge.end(); ++it2)
	{
		cout << it2->first;
	}
}
void cleanRead() {
	GgraphNode.clear();
	GgraphEdge.clear();
	QgraphNode.clear();
	QgraphEdge.clear();
}