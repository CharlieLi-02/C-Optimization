#include "Atlas.h"
#include <fstream>
#include <iostream>
#include "Station.h"
#include <stdexcept>


Atlas* Atlas::create(std::istream& stream) {
  // This default implementation will probably do what you want.
  // ���Ĭ��ʵ�ֿ��ܻ�����������Ҫ��
  // if you use a different constructor, you'll need to change it.
  // ���ʹ�ò�ͬ�Ĺ��캯��������Ҫ��������

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
    // ��������
    std::string name = "";
    std::string time_train;
    std::string name_train;
    std::string name_line;
    while (!stream.eof())
    {
        stream >> name;
        if ( !name.find("TRAIN:") || !name.find("BUS:")){
            vector<platform> from;
            std::getline(stream >> std::ws, name_line);
            station->mymap[name_line] = from;
            continue;
        }else if(!name.find("-")) {
            stream >> time_train;
            std::getline(stream >> std::ws, name_train);
            vector<platform> from = station->mymap[name_line];
            platform  pm;
            pm.timer =(short) atoi(time_train.c_str());
            pm.name = name_train;
            from.push_back(pm);
            vector<string>  as = fer[name_train];
            as.push_back(name_line);
            fer[name_train]= as;
            station->mymap[name_line] = from;
            continue;
        }
    }
    AMG->transfer = fer;
    //��������ͼ�ٽ����
    map<string, vector<platform>>  psm = station->mymap;
    int vName_id = 0;
    for (auto oc = psm.begin(); oc != psm.end(); oc++)
    {
        vector<platform>  plm = oc->second;
        platform  original = plm[0];
        int  m_vexNum = 0;
        int  m_arcNum = int(plm.size() - 1);
        int gid = -1; // ��ǰ�������
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
                    //���뽻���������
                    AMG->m_arcWeight[gid][gid] = 0;
                    gid = -1;
                }else {
                    AMG->m_arcWeight[vName.id][vName.id] = 0;
                }
                
            }else {
                int time = prm.timer - original.timer;
                if (gid != -1) {
                    //����ͬվ��
                    //������ڽ��棬���ٽ�����
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
    string src_1 = src;
    string dis_1 = dst;
    int start = locateVex(AMG, src_1);
    int stop =  locateVex(AMG, dis_1);
    if (start == -1 || stop == -1) {
        throw std::runtime_error("No route.");
    }
    dijastral(this, start, stop);
    return  *trip;
}

