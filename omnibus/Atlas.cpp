#include "Atlas.h"
#include <fstream>
#include <iostream>
#include "Station.h"
#include <stdexcept>
#include <chrono>

Atlas* Atlas::create(std::istream& stream) {
  // This default implementation will probably do what you want.
  // 这个默认实现可能会满足您的需要。
  // if you use a different constructor, you'll need to change it.
  // 如果使用不同的构造函数，则需要更改它。

  return new Atlas(stream);
}


Atlas::Atlas(std::istream& stream) {

    chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    cout << "当前时间" << ms.count() << endl;
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
        if ( !name.find("TRAIN:") || !name.find("BUS:")){
            vector<platform> *from=new vector<platform>;
            std::getline(stream>> std::ws, name_line);
            station->mymap.insert(pair<string, vector<platform>*>(name_line, from));
            if (!name.find("BUS:")) {
                T_B = "B";
            }
            if (!name.find("TRAIN:")) {
                T_B = "T";
            }
            continue;
        }else if(!name.find("-")) {
            stream >> time_train;
            std::getline(stream >> std::ws, name_train);
            vector<platform> *from = station->mymap[name_line];
            platform  pm;
            if (T_B.compare("B") == 0){
                pm.timer = 0;
            }
            else {
                pm.timer = atoi(time_train.c_str());
            }            
            pm.name = name_train;
            from->push_back(pm);
            vector<string>  as;
            for (auto oc = AMG->transfer.begin(); oc != AMG->transfer.end(); oc++)
            {
                if (oc->first.compare(name_train) == 0) {
                        as = oc->second;
                        AMG->transfer.erase(oc);
                        break;
                }
            }
            as.push_back(name_line);
            AMG->transfer.insert(pair<string,vector<string>>(name_train, as));
            station->mymap.insert(pair<string, vector<platform>*>(name_line, from));
            continue;
        }
    }
    //chrono::milliseconds stop = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "当前加载文件时间" <<(stop.count() - ms.count())<< endl;
    AMG->m_arcWeight = new int* [AMG->transfer.size()];
    for (size_t i = 0; i < AMG->transfer.size(); i++)
    {
        AMG->m_arcWeight[i] = new int[AMG->transfer.size()];
    }

    for (size_t i = 0; i < AMG->transfer.size(); i++) {
        for (size_t j = 0; j < AMG->transfer.size(); j++) {
            AMG->m_arcWeight[i][j] = QID;
        }
    }

    initDisPath((int)AMG->transfer.size());
    //chrono::milliseconds stop2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "当前初始化时间" << (stop2.count()- stop.count())<< endl;
    //AMG->transfer = fer;
    //构建有向图临界矩阵
    map<string, vector<platform>*>  psm = station->mymap;
    int vName_id = 0;
    for (auto oc = psm.begin(); oc != psm.end(); oc++)
    {
        vector<platform>  *plm = oc->second;
        platform  original = (*plm->begin());
        //int  m_arcNum = int(plm->size() - 1);
        int gid = -1; // 当前对象标量
        for(auto ac = plm->begin(); ac !=plm->end(); ac++)
        {
            vexName  *vName = new vexName;
            bool  flags = false;
            for (size_t j = 0; j < AMG->m_vexName.size(); ++j) {
                if (!(AMG->m_vexName[j]->name.compare(ac->name))) {
                    flags = true;
                    gid = AMG->m_vexName[j]->id;
                    break;
                }
            }
            if (!flags) {
                vName->name = ac->name;
                vName->id = vName_id++;
                AMG->m_vexName.push_back(vName);
            }
            if(original.name.compare(ac->name) == 0) {
                if (gid != -1) {
                    //插入交叉自身距离
                    AMG->m_arcWeight[gid][gid] =  -1;
                    gid = -1;
                }else {
                    AMG->m_arcWeight[vName->id][vName->id] =-1;
                }                
            }else {
                int  temp = 0;
                int time = ac->timer - original.timer;
                if (gid != -1) {
                    //有相同站点
                    //如果存在交叉，和临近距离
                    for (size_t k = 0; k < AMG->m_vexName.size(); k++) {
                        if (original.name.compare(AMG->m_vexName[k]->name) == 0) {
                            temp = AMG->m_vexName[k]->id;
                            break;
                        }
                    }
                    AMG->m_arcWeight[gid][temp] = time;
                    AMG->m_arcWeight[temp][gid] = time;
                    gid = -1;
                }else {
                    for (size_t k = 0; k < AMG->m_vexName.size(); k++) {
                        if (original.name.compare(AMG->m_vexName[k]->name) == 0) {
                            temp = AMG->m_vexName[k]->id;
                            break;
                        }
                    }
                    AMG->m_arcWeight[vName->id][temp] = time;
                    AMG->m_arcWeight[temp][vName->id] = time;
                }
            }
            original = (*ac);
        }      
        //AMG->m_arcNum = AMG->m_arcNum + m_arcNum;
    }
    AMG->m_vexNum = (int)AMG->transfer.size();
    //chrono::milliseconds stop3 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "构建矩阵时间" << (stop3.count() - ms.count()) << endl;
}

Atlas::~Atlas() {
    map<string, vector<platform>*>  psm = station->mymap;
    for (auto oc = psm.begin(); oc != psm.end();)
    {
        delete oc->second;
        oc->second = NULL;
        psm.erase(oc++);
    }
    delete station;
    station = NULL;
    for (int i = 0; i < 250; i++)
    {
        delete[] AMG->m_arcWeight[i];
    }
    delete[] AMG->m_arcWeight;
    AMG->m_arcWeight = NULL;
    for (auto iter = AMG->m_vexName.begin(); iter != AMG->m_vexName.end(); ++iter)
    {
        if (*iter != NULL)
        {
            delete (*iter);
            (*iter) = NULL;
        }
    }
    AMG->m_vexName.clear();
    delete AMG;
    AMG = NULL;
    delete trip;
    trip = NULL;
    void disDe();
}


Trip Atlas::route(const std::string& src, const std::string& dst) {
    //string src_1 = src;
    //string dis_1 = dst;
    int start = locateVex(AMG, src);
    int stop =  locateVex(AMG, dst);
    if (start == -1 || stop == -1) {
        throw std::runtime_error("No route.");
    }
    chrono::milliseconds s_1 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    dijastral(this, start, stop);
    chrono::milliseconds s_2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    cout << "计算时间" << (s_2.count() - s_1.count()) << endl;
    return  *trip;
}

