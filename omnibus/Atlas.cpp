#include "Atlas.h"
#include <fstream>
#include <iostream>
#include "Station.h"
#include <stdexcept>
#include <chrono>

Atlas* Atlas::create(std::istream& stream) {
  // This default implementation will probably do what you want.
  // if you use a different constructor, you'll need to change it.

  return new Atlas(stream);
}


Atlas::Atlas(std::istream& stream) {

    station = new Station();
    trip = new Trip();
    AMG = new AMGGraph();
    //load input
    std::string name = "";
    std::string time_train;
    std::string name_train;
    std::string name_line;
    string T_B = "T";
    // read input
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
            if (from == NULL) {
                continue;
            }
            platform  pm;
            if (T_B.compare("B") == 0){
                pm.timer = 0;
            }
            else {
                pm.timer = (short)atoi(time_train.c_str());
            }            
            pm.name = name_train;
            from->push_back(pm);           
            map<string, vector<string>>::iterator as =AMG->transfer.find(name_train);
            if (as!= AMG->transfer.end()){
                as->second.push_back(name_line);
            }else{
                vector<string>  ast;
                ast.push_back(name_line);
                AMG->transfer.insert(pair<string, vector<string>>(name_train, ast));
            }  
            station->mymap.insert(pair<string, vector<platform>*>(name_line, from));
            continue;
        }
    }
    AMG->m_arcWeight = new short* [AMG->transfer.size()];
    for (size_t i = 0; i < AMG->transfer.size(); i++)
    {
        AMG->m_arcWeight[i] = new short[AMG->transfer.size()];
    }

    for (size_t i = 0; i < AMG->transfer.size(); i++) {
        for (size_t j = 0; j < AMG->transfer.size(); j++) {
            AMG->m_arcWeight[i][j] = QID;
        }
    }

    initDisPath((short)AMG->transfer.size());
    chrono::milliseconds stop2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //build ordered adjacency matrix
    map<string, vector<platform>*>  psm = station->mymap;
    short vName_id = 0;
    for (auto oc = psm.begin(); oc != psm.end(); oc++)
    {
        vector<platform>  *plm = oc->second;
        if (plm == NULL) {
            continue;
        }
        platform  original = (*plm->begin());
        int gid = -1; // current target index
        for(auto ac = plm->begin(); ac !=plm->end(); ac++)
        {
            vexName* vName = new vexName;
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
            else {
                delete vName;
                vName = NULL;
            }
            if(original.name.compare(ac->name) == 0) {
                if (gid != -1) {
                    //insert self-cross distance
                    AMG->m_arcWeight[gid][gid] =  -1;
                    gid = -1;
                }else {
                    if (vName != NULL) {
                        AMG->m_arcWeight[vName->id][vName->id] = -1;
                    }                  
                }                
            }else {
                int temp = 0;
                int time = ac->timer - original.timer;
                if (gid != -1) {
                    //if same station exists
                    //if cross exist and close distance exist
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
                    if (vName != NULL) {
                        AMG->m_arcWeight[vName->id][temp] = time;
                        AMG->m_arcWeight[temp][vName->id] = time;
                    }                    
                }
            }
            original = (*ac);
        }      
    }
    AMG->m_vexNum = (short)AMG->transfer.size();
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
    for (size_t i = 0; i < AMG->transfer.size(); i++)
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
    if (AMG != NULL) {
        delete AMG;
        AMG = NULL;
    }
    if (trip != NULL) {
        delete trip;
        trip = NULL;
    }
    void disDe();
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

