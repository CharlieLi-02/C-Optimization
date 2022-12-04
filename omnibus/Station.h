#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

struct platform {
	short timer=0;
	string name;
};

struct Station {
	map<string,vector<platform>*>  mymap;
};

