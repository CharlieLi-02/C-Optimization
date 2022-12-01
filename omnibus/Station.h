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
    string startName; //
    string stopName;  //
    int  direction = 0; //
    string line;   //
    int times = 0;
}; //

class Station {
  public:
	map<string,vector<platform>>  mymap;
	map<int,vector<line_1>>        lines;//
};

