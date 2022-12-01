#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;


class platform {
    public: 
		 short timer=0;
	     string name;
};

class line_1 {
 public:
    string startName; // 起始名称
    string stopName;  // 结束名称
    int  direction = 0; // 方向
    string line;   //线路名称
    int times = 0;
}; //线路

class Station {
  public:
	map<string,vector<platform>>  mymap;
	map<int,vector<line_1>>        lines;// 线路集合
};

