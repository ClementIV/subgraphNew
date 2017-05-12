#pragma once
#include "readfile.h"
#include "CFL.h"
// retrun the root
int getRoot();
//return maximum neighbor-degree of  v and G
int mndG( int v);
//return maximum neighboe-degree of u and q
int mndq( int u);
//return degree of v and label l
int dG(int v, string l);
// return degree of u and label l
int dq(int u, string l);
// input: A potential candiate vertex v for a query vertex u
//outout: true if v is a candidate of u,and false otherwise
bool CandVerify(int v, int u);  
bool ValidateNT(int v, int u,map<int,int>M);