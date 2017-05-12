#include "CFL.h"
#include "CandVerify.h"
#include "CPI.h"
#include "writefile.h"

extern map<int, map<int, int>>MLeaf;
extern map<int, GNode>GgraphNode;
extern map<int, GEdge>GgraphEdge;
extern map<int, qNode>QgraphNode;
extern map<int, qEdge>QgraphEdge;
extern map<int, qEdge>QgraphEdge2;
int main() 
{
	string path = "datas";
	path = path + char(92)+ "Californa" + char(92);//Californianode Californiaedge Qnode Qedge
	string GN ="GNode";
	string GE = "GEdge";
	string qN = "qNode";
	string qE = "qEdge";
	string As = "answer";
	string suffix =".txt";
	int i = 1;
	int end = 1;
	while(i<=end)
	{
		//cout << i <<": ";
		seach(GN + char(i + 48)+suffix, GE + char(i + 48) +suffix, qN + char(i+48) +suffix, qE+char(i+48) + suffix, As + char(i+48) + suffix);
		i++;
		//cleanAll();
	
	}
}
void seach(string GN, string GE, string qN, string qE,string Answer)
{
	getData(GN,  GE,  qN,  qE);
	CFL_Descompose();
	BottomUp();
	MatchAll();
	for (map<int, map<int, int>>::iterator it = MLeaf.begin(); it != MLeaf.end(); ++it)
	{
		cout << it->first << ":";
		for (map<int, int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			cout << it2->first << "->" << it2->second << " ";
		}
		cout << endl;
	}
	writeAnswer(Answer);
	if (check(Answer))
		cout << "success!" << endl;
	else
		cout << "defeat!" << endl;
}
void writeAnswer(string answer) 
{
	ofstream AnswerFile(answer,ios::out);
	for (map<int, map<int, int>>::iterator it = MLeaf.begin(); it != MLeaf.end(); ++it)
	{
		AnswerFile << it->first << ": "; 
			for (map<int, int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				AnswerFile << it2->first << " -> " << it2->second << " ";
			}
		AnswerFile << endl;
	}
}
bool check(string answer)
{
	int num;
	string link;
	int u, v;
	int i;
	bool result = true;
	map<int, int> M;
	//map<int, int> uM;
	fstream an(answer);
	while (an >> num)
	{
		an >> link;
		M.clear();
		for (i = 0; i < QgraphEdge.size(); i++) 		{
			
			an >> u >> link >> v;
			M.insert(pair<int, int>(u, v));
		}
		for (map<int, int>::iterator it = M.begin(); it != M.end(); ++it)// it mean each u-v map | it->begin() means each u-v
		{
			for (set<int>::iterator it2 = QgraphEdge[it->first].Edges.begin(); it2 != QgraphEdge[it->first].Edges.end(); ++it2)//it2 means each nebor u' of u in q 
			{
				if (GgraphEdge[it->second].Edges.find(M[*it2]) == GgraphEdge[it->second].Edges.end())
				{
					return false;
				}
			}
		}
		//cout << num << " true" << endl;
	}
	return true;
}
void cleanAll()
{
	cleanRead();
	cleanCFL();
	cleanCPI();
	cleanMatch();
}