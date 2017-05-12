#pragma once
#include "readfile.h"
#include "Candverify.h"

//extern map<int, qEdge>QgraphEdge2;
void TopDown();
void BFS_Level();
void BottomUp();
void MatchOrderCore();
typedef struct _RTL 
{
	vector<int> vertex;
	int NT;
}RTL;
int C(vector<int> pi);
void CoreMatch(int i,map<int,int>MC);
void GetcoreTree();
void getCorePi(RTL rtl, int r);
void getForestPi(vector<int> vertex,int r);
void ForestMatch(int i,map<int,int>MF,map<int, int>MC);
void getLeafPi(vector<int> vertex, int r);
void MatchAll();
void cleanMatch();
void cleanCPI();