#include "Atlas.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <algorithm>

extern vector<string>  app;
extern int index;

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
    //chrono::milliseconds start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
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
            map<string, vector<platform>*>::iterator from = station->mymap.find(name_line);
            if (from != station->mymap.end()) {
                platform  pm;
                if (T_B.compare("B") == 0) {
                    pm.timer = 0;
                }
                else {
                    pm.timer = atoi(time_train.c_str());
                }
                pm.name = name_train;
                from->second->push_back(pm);
                map<string, vector<string>>::iterator as = AMG->transfer.find(name_train);
                if(as != AMG->transfer.end()) {                    
                   vector<string>::iterator iter = std::find(as->second.begin(), as->second.end(), name_line);
                   if (iter == as->second.end()) {
                       //去掉环形站点对应值
                       as->second.push_back(name_line);
                   }                  
                }
                else {
                    vector<string>  ast;
                    ast.push_back(name_line);
                    AMG->transfer.insert(pair<string, vector<string>>(name_train, ast));
                }
                station->mymap.insert(pair<string, vector<platform>*>(name_line, from->second));
            }
            continue;
        }
    }
    //chrono::milliseconds stop = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "加载文件时间" << (stop.count() - start.count()) << endl;
    G = (AGraph*)malloc(sizeof(AGraph));
    int vName_id = 0;
    for (auto oc = station->mymap.begin(); oc != station->mymap.end(); oc++)
    {
        vector<platform>* plm = oc->second;
        AMG->m_vexNum = int(AMG->m_vexNum+ plm->size());
        for (auto ac = plm->begin(); ac != plm->end(); ac++) {
            AMG->m_vexName.insert(pair<int, string >(vName_id, oc->first+"/"+ ac->name));
            AMG->m_vexId.insert(pair<string, int>(oc->first + "/" + ac->name, vName_id));
            vName_id++;
        }
    }
    CreateGraph(G, this);
    //chrono::milliseconds stop2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "构建邻接表时间" << (stop2.count() - stop.count()) << endl;

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

    for (auto oc = AMG->node.begin(); oc != AMG->node.end();)
    {
        if (oc->second != nullptr) {
            delete[] oc->second;
            oc->second = NULL;
        }
        AMG->node.erase(oc++);
    }

    AMG->node.clear();

    if (AMG != NULL) {
        delete AMG;
        AMG = NULL;
    }

    for (int i = 0; i < G->numNodes; i++) {
        ArcNode* freeNode;
        while (NULL != G->adjlist[i].firstarc) {
            freeNode = G->adjlist[i].firstarc;
            G->adjlist[i].firstarc = G->adjlist[i].firstarc->nextarc;
            free(freeNode);
        }
    }

    if (G->adjlist != NULL){
        delete [] G->adjlist;
        G->adjlist = NULL;
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
    
    map<string, vector<string>>::iterator src1 = AMG->transfer.find(src);
    map<string, vector<string>>::iterator dst1 = AMG->transfer.find(dst);
    if (src1 == AMG->transfer.end() || dst1 == AMG->transfer.end() ) {
        throw std::runtime_error("No route.");
    }
    vector<string> src1_1 = src1->second;
    vector<string> dst1_1 = dst1->second;
    int stop = -1;
    int start = -1;
    for (int i = 0; i<src1_1.size(); i++) {
        string src_1 = src1_1[i] + "/" + src;
        start = locateVex(AMG, src_1);
        for (int j = 0; j < dst1_1.size(); j++) {
            string dst_1 = dst1_1[j] + "/" + dst;
            stop = locateVex(AMG, dst_1);    
            Dijkstra2(this, start, stop);        
        }
    }
    map<int, int*>::iterator as = AMG->path.find(index);
    if (as != AMG->path.end()) {
        initTrip(trip);
        print_line_2(this,as->second, stop);
    }
    index = -1;
    //chrono::milliseconds s_1 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());

    //chrono::milliseconds s_2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "计算时间" << (s_2.count() - s_1.count()) << endl;
    return  *trip;
}

