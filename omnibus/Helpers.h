#ifndef HELPERS_H
#define HELPERS_H

// If you want to add any helper classes,
// here's some space to do it in.

#include <string>
#include <map>
#include <vector>
#include "Trip.h"
#define MAXVEX 50000
#define maxSize 12520
#define INF 32677

using namespace std;


struct platform {
	short timer = 0;
	string name;
};

struct Station {
	map<string, vector<platform>*>  mymap;
};

typedef struct ArcNode
{
	int weight = 0;		//��¼Ȩֵ
	int adjvex = 0;		//�ڽӵ�
	struct ArcNode* nextarc;
}ArcNode;
typedef struct VNode
{
	int data = 0;
	ArcNode* firstarc;
}VNode;
typedef struct AGraph
{
	VNode adjlist[MAXVEX];
	int numNodes, numEdges;
}AGraph;


//����ڵ㣬����id�͵�Դ����Ĺ�����룬���ȶ�����Ҫ������
struct Node
{
    int id;     //��Դ��Ŀ�궥��id
    int w;      //��Դ��id�ľ���

    //��Ҫʵ����С�ѣ����������У������Ҫ������������ض������ȼ���ʹ�ý������
    friend bool operator < (struct Node a, struct Node b)
    {
        return a.w > b.w;
    }
};

struct vexName {
    string name;
    int id = 0;
    bool operator == (const string& pos) {
        return ((this->name).compare(pos) == 0);
    }
};

typedef struct finder_t
{
    finder_t(string _name)
        :name(_name)
    {}
    //����()--->cpp�ķº������溯��ָ��
    bool operator()(vexName* f)
    {
        return (name == f->name);
    }

    string name;
}finder_t;


struct AMGGraph {
    short m_vexNum = 0, m_arcNum = 0;//
    vector<vexName*> m_vexName; //
    map<string, vector<string>>   transfer; //
};

class Atlas;

// ------
int locateVex(AMGGraph* AMG, string vexName);
string localteVex(AMGGraph* AMG, int vexId);
void initTrip(Trip* trips);
vector<string> localteStatic(Atlas* atlas, int vexId);
platform  getPlatform(vector<platform>  platform_1, string name);
bool  ThreeStation(vector<string> vec, string name);
vector<platform> localteStation(Atlas* atlas, string name);
bool Onlineandoffline(Atlas* atlas, string lines, int j);
void intDijkstra(Atlas* atlas, int start, int stop);
void CreateGraph(AGraph* G, Atlas* atlas);
void Dijkstra(Atlas* AT, int dist[], int path[], int v, int stop);
int getWeight(AGraph* G, int u, int j);
void print_path(AMGGraph* AMG, int path[], int v1);
void DFSPrint(AMGGraph* AMG, int s, int v, int path[]);
void  print_line(Atlas* atlas, int start);
// ------
void Dijkstra2(Atlas* AT, int v, int stop);
void Dijkstra(Atlas* AT, int v0, int n);
#endif
