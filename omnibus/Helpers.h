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
#include "Station.h"
class Atlas;


using namespace std;

struct vexName {
    string name;
    int id=0;
};

struct AMGGraph {
    int m_vexNum=0 , m_arcNum =0;//
    vector<vexName> m_vexName; //
    int m_arcWeight[100][100] = { 1500 };   //
    map<string, vector<string>>   transfer; //
};

void Stringsplit(const string& str, const string& splits, vector<string>& res);
int locateVex(AMGGraph *AMG, string vexName);
string localteVex(AMGGraph* AMG, int vexId);
vector<string> localteStatic(Atlas* atlas, int vexId);
platform  getPlatform(vector<platform>  platform_1, string name);
void  graphlines(Atlas* atlas, int start, int stop);
void showPath(AMGGraph* AMG, int startVexAdd, int endVexAdd);
void dijastral(Atlas* atlas, int start, int stop);
void disDelete();
#endif
