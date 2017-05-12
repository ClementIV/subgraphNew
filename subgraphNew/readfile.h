#pragma once
#include<fstream>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <map>
#include <list>
//#define GNodefile "GNode"
//#define GEdgefile "GEdge"
//#define qNodefile "qNode"
//#define qEdgefile "qEdge"
using namespace std;
typedef struct _GNode
{
	string label;
	int degree;
	int cnt;
	bool visted;
}GNode;
typedef struct _GEdge
{
	set<int> Edges;

}GEdge;
 typedef struct _qNode
{
	string label;
	int degree;
	int cnt;
	bool visted;
	int p;
}qNode;
 typedef struct _qEdge
{
	set<int> Edges;
}qEdge;

//static map<qNode,int>::iterator it;
 void getData(string GN, string GE, string qN, string qE);
void print();
void cleanRead();