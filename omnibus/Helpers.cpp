#include "Helpers.h"
#include "Atlas.h"
#include "Station.h"

// Space to implement helper class member functions.

const int PI = 32677;
int Distance[100] = { 0 };
int path[100] = { 0 };
bool s[100] = { 0 };
vector<string>  app;

void Stringsplit(const string& str, const string& splits, vector<string>& res)
{
    if (str == "") {
        return;
    }
    //
    string strs = str + splits;
    size_t pos = strs.find(splits);
    int step = (int)splits.size();
    //
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        //
        strs = strs.substr(pos + step, strs.size());
        pos = strs.find(splits);
    }
}

//
int locateVex(AMGGraph *AMG, string vexName) {
    for (int i = 0; i < AMG->m_vexNum; i++) {
        if (AMG->m_vexName[i].name.compare(vexName) == 0) {
            return AMG->m_vexName[i].id;
        }
    }
    return -1;
}

//
string localteVex(AMGGraph* AMG, int vexId) {
    for (int i = 0; i < AMG->m_vexNum; i++) {
        if (AMG->m_vexName[i].id== vexId) {
            return AMG->m_vexName[i].name;
        }
    }
    return "no";
}

platform  getPlatform(vector<platform>  platform_1,string name) {
    platform  form;
    for (auto i= 0; i < platform_1.size(); i++) {
        if (platform_1[i].name.compare(name)) {
            form = platform_1[i];
        }
    }
    return form;
}

//
void  graphlines(Atlas* atlas, int start, int stop) {
    AMGGraph* amg = atlas->AMG;
    Station* station = atlas->station;
    map<int, vector<line_1>>  aline = station->lines;
    vector<string> startT = localteStatic(atlas, start);
    vector<string> stopT = localteStatic(atlas, stop);
    //
    //
    for (auto i = 0;  i < startT.size();  i++) {
        for (auto j = 0; j < stopT.size(); j++) {
            if (startT[i].compare(stopT[j]) == 0) {
                line_1 li;
                 li.line = startT[i];
                 li.startName = localteVex(amg, start);
                 li.stopName =  localteVex(amg, stop);
                 vector<platform> plform =station->mymap[li.line];
                 platform startform = getPlatform(plform, li.startName);
                 platform stopform = getPlatform(plform, li.stopName);
                 if (startform.timer > stopform.timer) {
                     li.direction = 1;
                     li.times = startform.timer - stopform.timer;
                 }
                 else {
                     li.direction = 0;
                     li.times = stopform.timer - startform.timer;
                 }
                 vector<line_1>  vli;
                 vli.push_back(li);
                 (aline)[0] = vli;
                 return;
             }else{
                         
            }
        }
    }

    
}

//
vector<string> localteStatic(Atlas* atlas,int vexId) {
    AMGGraph* amg = atlas->AMG;
    string name =localteVex(amg, vexId);
    vector<string>  names = amg->transfer[name];
    return names;
}

//
void initTrip(Trip * trips) {
    trips->legs.clear();
    app.clear();
}

void dijastral(Atlas* atlas, int start, int stop){

    
    AMGGraph* amg = atlas->AMG;
    Trip*  trips =atlas->trip;
    //
    initTrip(trips);
    for (int i = 0; i < amg->m_vexNum; i++) {
         Distance[i] = amg->m_arcWeight[start][i];
         if (Distance[i] > 0 && Distance[i]!=1500) {
             path[i] = start;
         }else {
             path[i] = -1;
         }
    }
    s[start] = true;
    Distance[start] = 0;
    int min = 100000;
    int pos = 0; //
    //
    for (int i = 1; i < amg->m_vexNum; i++) {
        //
        min = PI;
        for (int j = 0; j < amg->m_vexNum; j++) {
         if (!s[j] && Distance[j] < min) {
                pos = j;
                min = Distance[j];  //
            }
           
        }
        s[pos] = true;
        for (auto k = 0; k < amg->m_vexNum; k++) {
            if (!s[k] && Distance[k] > amg->m_arcWeight[pos][k] + Distance[pos]) {
                int ace = amg->m_arcWeight[pos][k] + Distance[pos];
                Distance[k] = ace;
                path[k] = pos;
            }
        }
    }   
    showPath(amg, start, stop);

    app.push_back(localteVex(amg, stop));
    //
    trips->start = localteVex(amg, start);
    //
    for (int i = 0 ; i <  app.size();  i++) {
        Trip::Leg  lgs;
        vector<string> vec = amg->transfer[app[i]];
        if (vec.size() == 1) {
            //
            lgs.line = vec[0];
            bool flags = false;
            for (auto j = (i+1); j < app.size(); j++) {
                vector<string> vec2 = amg->transfer[app[j]];
                if (vec2.size() > 1 &&  j!= (app.size()-1)) {
                    //
                    vector<string> vec3 = amg->transfer[app[j+1]];
                    for (auto k = 0; k < vec3.size(); k++) {
                        if (vec3[k].compare(lgs.line) == 0) {
                            flags = true;
                            break;
                        }
                    }
                    if (!flags) {
                        //
                        lgs.stop = app[j];
                        flags = false;
                        break;
                    }
                    flags = false;
                }
                i = j;
            }
            if (i == (app.size() - 1)) {
                //
                lgs.stop = app[app.size() - 1];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
        else {
            //
            int aps = i + 1;
            bool flags = false;
            //
            vector<string> vec2 = amg->transfer[app[(aps)]];
            for (auto j = 0; j < vec.size(); j++) {
                for (int k = 0; k < vec2.size(); k++) {
                    if (vec[j].compare(vec2[k]) == 0) {
                        lgs.line = vec[j];
                        break;
                    }
                }
            }

            for (auto j = aps; j <app.size(); j++) {
                vector<string> vec2 = amg->transfer[app[(j)]];
                if (vec2.size() > 1 && j != (app.size() - 1)) {
                    vector<string> vec3 = amg->transfer[app[j + 1]];
                    for (int k = 0; k < vec3.size(); k++) {
                        if (vec3[k].compare(lgs.line) == 0) {
                            flags = true;
                            break;
                        }
                    }
                    if (!flags) {
                        lgs.stop = app[j];
                        flags = false;
                        break;
                    }
                    flags = false;
                }
                i = j;
            }
            if (i==(app.size()-1)) {
                lgs.stop = app[app.size() - 1];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
    }
    disDelete();
}


void showPath(AMGGraph *AMG, int startVexAdd, int endVexAdd) {
    if (path[endVexAdd] != -1) {
        showPath(AMG, startVexAdd, path[endVexAdd]);
        string vex = localteVex(AMG, path[endVexAdd]);
        //AMG->transfer[vex]
        app.push_back(vex);
    }
}

void disDelete() {   
     for (int i = 0; i < 100; i++) {
         Distance[i] = 0;
         path[i] = 0;
         s[i] = false;
     }
}
