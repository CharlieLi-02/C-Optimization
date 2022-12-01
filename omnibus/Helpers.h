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
#include <cstddef>
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
string localteVex(AMGGraph* AMG, size_t vexId);
vector<string> localteStatic(Atlas* atlas, size_t vexId);
platform  getPlatform(vector<platform>  platform_1, string name);
void  graphlines(Atlas* atlas, size_t start, size_t stop);
void showPath(AMGGraph* AMG, size_t startVexAdd, size_t endVexAdd);
void dijastral(Atlas* atlas, size_t start, size_t stop);
void disDelete();
#endif
