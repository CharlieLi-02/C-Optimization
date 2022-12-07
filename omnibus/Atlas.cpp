#include "Atlas.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <algorithm>

extern vector<string>  app;

Atlas* Atlas::create(std::istream& stream) {
    // This default implementation will probably do what you want.
    // 这个默认实现可能会满足您的需要。
    // if you use a different constructor, you'll need to change it.
    // 如果使用不同的构造函数，则需要更改它。

    return new Atlas(stream);
}


Atlas::Atlas(std::istream& stream) {

    //chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "当前时间" << ms.count() << endl;
    station = new Station();
    trip = new Trip();
    AMG = new AMGGraph();
    //加载数据
    std::string name = "";
    std::string time_train;
    std::string name_train;
    std::string name_line;
    string T_B = "T";
    // 读文件
    while (!stream.eof())
    {
        stream >> name;
        if (!name.find("TRAIN:") || !name.find("BUS:")) {
            vector<platform>* from = new vector<platform>;
            std::getline(stream >> std::ws, name_line);
            station->mymap.insert(pair<string, vector<platform>*>(name_line, from));
            if (!name.find("BUS:")) {
                T_B = "B";
            }
            if (!name.find("TRAIN:")) {
                T_B = "T";
            }
            continue;
        }
        else if (!name.find("-")) {
            stream >> time_train;
            std::getline(stream >> std::ws, name_train);
            if (name_line == "") {
                continue;
            }
            vector<platform>* from = station->mymap[name_line];
            platform  pm;
            if (T_B.compare("B") == 0) {
                pm.timer = 0;
            }
            else {
                pm.timer = (short)atoi(time_train.c_str());
            }
            pm.name = name_train;
            from->push_back(pm);
            map<string, vector<string>>::iterator as = AMG->transfer.find(name_train);
            if (as != AMG->transfer.end()) {
                as->second.push_back(name_line);
            }
            else {
                vector<string>  ast;
                ast.push_back(name_line);
                AMG->transfer.insert(pair<string, vector<string>>(name_train, ast));
            }
            station->mymap.insert(pair<string, vector<platform>*>(name_line, from));
            continue;
        }
    }
    chrono::milliseconds stop = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    G = (AGraph*)malloc(sizeof(AGraph));
    AMG->m_vexNum = (short)AMG->transfer.size();
    CreateGraph(G, this);
    chrono::milliseconds stop2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    cout << "构建邻接表时间" << (stop2.count() - stop.count()) << endl;
}


Atlas::~Atlas() {
    map<string, vector<platform>*>  psm = station->mymap;
    for (auto oc = psm.begin(); oc != psm.end();)
    {
        if (oc->second != nullptr) {
            delete oc->second;
            oc->second = NULL;
        }
        psm.erase(oc++);
    }
    psm.clear();
    delete station;
    station = NULL;

    for (auto oc = AMG->path.begin(); oc != AMG->path.end();)
    {
        if (oc->second != nullptr) {
            delete [] oc->second;
            oc->second = NULL;
        }
        AMG->path.erase(oc++);
    }

    AMG->path.clear();

    if (AMG != NULL) {
        delete AMG;
        AMG = NULL;
    }

    for (int i = 0; i < G->numNodes; i++) {
        ArcNode* node = G->adjlist[i].firstarc;
        while (node != NULL)
         {
            ArcNode* node1 = node->nextarc;
            free(node);
            node = node1;
         }
    }

    if (G != NULL) {
        free(G);
        G = NULL;
    }

    if (trip != NULL) {
        delete trip;
        trip = NULL;
    }
}


Trip Atlas::route(const std::string& src, const std::string& dst) {
    int start = locateVex(AMG, src);
    int stop = locateVex(AMG, dst);

    if (start == -1 || stop == -1) {
        throw std::runtime_error("No route.");
    }
    chrono::milliseconds s_1 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    map<int, int*>::iterator as = AMG->path.find(start);
    if (as != AMG->path.end()){
          app.clear();
          int *path =as->second;
          app.push_back(localteVex(AMG, stop));
          while (path[stop] != -1) {
              app.push_back(localteVex(AMG, path[stop]));
              stop = path[stop];
          }
          if (app.size() == 1) {             
              throw std::runtime_error("No route.");
          }
          reverse(app.begin(), app.end());
          print_line(this, start);
    }
    else {
        Dijkstra2(this, start, stop);
    }
    chrono::milliseconds s_2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    cout << "计算时间" << (s_2.count() - s_1.count()) << endl;
    return  *trip;
}

