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
	int weight = 0;		//记录权值
	int adjvex = 0;		//邻接点
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


//顶点节点，保存id和到源顶点的估算距离，优先队列需要的类型
struct Node
{
    int id;     //从源到目标顶点id
    int w;      //从源到id的距离

    //因要实现最小堆，按升序排列，因而需要重载运算符，重定义优先级，使得结点升序
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
    //重载()--->cpp的仿函数代替函数指针
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
