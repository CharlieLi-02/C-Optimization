#include "Atlas.h"
#include <fstream>
#include <iostream>
#include "Station.h"
#include <stdexcept>


Atlas* Atlas::create(std::istream& stream) {
  // This default implementation will probably do what you want.
  // 这个默认实现可能会满足您的需要。
  // if you use a different constructor, you'll need to change it.
  // 如果使用不同的构造函数，则需要更改它。

  return new Atlas(stream);
}


Atlas::Atlas(std::istream& stream) {
    station = new Station();
    trip = new Trip();
    AMG = new AMGGraph();
    map<string, vector<string>> fer = AMG->transfer;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            AMG->m_arcWeight[i][j] = QID;
        }
    }
    // 加载数据
    std::string line;
    string name = "";
    while (!stream.eof())
    {
        getline(stream, line);
        if ( !line.find("TRAIN:") || !line.find("BUS:")){
            vector<string> train;
            Stringsplit(line, ":", train);
            vector<platform> from;
            trim(train[1]);
            station->mymap[train[1]] = from;
            name = train[1];
        }else if(!line.find("-")) {
            vector<string> train;
            Stringsplit(line, "\t", train);
            Stringsplit(train[0], " ", train);
            vector<platform> from = station->mymap[name];
            platform  pm;
            pm.timer =(short) atoi(train[3].c_str());
            trim(train[1]);
            pm.name = train[1];
            from.push_back(pm);
            vector<string>  as = fer[train[1]];
            as.push_back(name);
            fer[train[1]]= as;
            station->mymap[name] = from;
        }      
    }
    AMG->transfer = fer;
    //构建有向图临界矩阵
    map<string, vector<platform>>  psm = station->mymap;
    int vName_id = 0;
    for (auto oc = psm.begin(); oc != psm.end(); oc++)
    {
        vector<platform>  plm = oc->second;
        platform  original = plm[0];       
        int  m_vexNum = 0;
        int  m_arcNum = int(plm.size() - 1);
        int gid = -1; // 当前对象标量
        for(size_t i = 0; i < plm.size(); ++i)
        {
            platform  prm = plm[i];
            vexName  vName;
            bool  flags = false;
            for (size_t j = 0; j < AMG->m_vexName.size(); ++j) {
                if (!(AMG->m_vexName[j].name.compare(prm.name))) {
                    flags = true;
                    gid = AMG->m_vexName[j].id;
                    break;
                }
            }
            if (!flags) {
                vName.name = prm.name;
                vName.id = vName_id++;
                m_vexNum++;
                AMG->m_vexName.push_back(vName);
            } 
            if (original.name.compare(prm.name) == 0) {
                if (gid != -1) {
                    //插入交叉自身距离
                    AMG->m_arcWeight[gid][gid] = 0;
                    gid = -1;
                }else {
                    AMG->m_arcWeight[vName.id][vName.id] = 0;
                }
                
            }else {
                int time = prm.timer - original.timer;
                if (gid != -1) {
                    //有相同站点
                    //如果存在交叉，和临近距离
                    int  temp = 0;
                    for (size_t k = 0; k < AMG->m_vexName.size(); k++) {
                        if (original.name.compare(AMG->m_vexName[k].name) == 0) {
                            temp = AMG->m_vexName[k].id;
                            break;
                        }
                    }                    
                    AMG->m_arcWeight[gid][temp] = time;
                    AMG->m_arcWeight[temp][gid] = time;
                    gid = -1;
                }else {
                    //if (vName.id == 28) {
                    //    cout << "测试" << endl;
                    //}
                    int  temp = 0;
                    for (size_t k = 0; k < AMG->m_vexName.size(); k++) {
                        if (original.name.compare(AMG->m_vexName[k].name) == 0) {
                            temp = AMG->m_vexName[k].id;
                            break;
                        }
                    }
                    AMG->m_arcWeight[vName.id][temp] = time;
                    AMG->m_arcWeight[temp][vName.id] = time;                  
                }              
            }           
            original = prm;
        }
        AMG->m_vexNum = AMG->m_vexNum + m_vexNum;
        AMG->m_arcNum = AMG->m_arcNum + m_arcNum;
    }
}

Atlas::~Atlas() {
    delete station;
    delete AMG;
    delete trip;
}


Trip Atlas::route(const std::string& src, const std::string& dst) {
    int start = locateVex(AMG, src);
    int stop =  locateVex(AMG, dst);
    if (start == -1 || stop == -1) {
        throw std::runtime_error("No route.");
    }
    dijastral(this, start, stop);
	return  *trip;
}

