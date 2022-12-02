#ifndef HELPERS_H
#define HELPERS_H

// If you want to add any helper classes,
// here's some space to do it in.
#include <istream>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include <cstddef>
#include "Station.h"

const int PI = 32677;
const int QID = 12520; // 最大距离


class Atlas;


using namespace std;

struct vexName {
    string name;
    int id=0;
};

struct AMGGraph {
    int m_vexNum=0 , m_arcNum =0;// 总顶点数 总边数
    vector<vexName> m_vexName; //顶点表
    int m_arcWeight[100][100] = { 12520 };   //邻接矩阵，边表
    map<string, vector<string>>   transfer; //换乘
};

void Stringsplit(const string& str, const string& splits, vector<string>& res);
int locateVex(AMGGraph *AMG, string vexName);
string localteVex(AMGGraph* AMG, int vexId);
vector<string> localteStatic(Atlas* atlas, int vexId);
platform  getPlatform(vector<platform>  platform_1, string name);
bool  ThreeStation(vector<string> vec, string name);
vector<platform> localteStation(Atlas* atlas, string name);
bool Onlineandoffline(Atlas* atlas, string lines, int j);
void  graphlines(Atlas* atlas, int start, int stop);
void showPath(AMGGraph* AMG, int startVexAdd, int endVexAdd);
void dijastral(Atlas* atlas, int start, int stop);
void disDelete();
#endif
