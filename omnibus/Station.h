#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

struct platform {
	short timer=0;
	string name;
};

struct line_1 {
    string startName; // ��ʼ����
    string stopName;  // ��������
    int  direction = 0; // ����
    string line;   //��·����
    int times = 0;
}; //��·

struct Station {
	map<string,vector<platform>*>  mymap;
	map<int,vector<line_1>>        lines;// ��·����
};

