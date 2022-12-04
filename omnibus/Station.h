#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

struct platform {
	int timer=0;
	string name;
};

struct Station {
	map<string,vector<platform>*>  mymap;
};

