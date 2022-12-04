#include "Helpers.h"
#include "Atlas.h"
#include "Station.h"

// Space to implement helper class member functions.

short* Distance =NULL;
short* path = NULL; 
bool* s = NULL;
vector<string>  app;
int  startSize = 0;
void  initDisPath(int size) {
    Distance = new short[size];
    path = new short[size];
    s = new bool[size];
    startSize = size;
    for (int i = 0; i < size; i++) {
        Distance[i] = 0;
        path[i] = 0;
        s[i] = false;
    }
}

int locateVex(AMGGraph *AMG, string vexName) {
    for (int i = 0; i < AMG->m_vexNum; i++) {
        if (AMG->m_vexName[i]->name.compare(vexName) == 0) {
            return AMG->m_vexName[i]->id;
        }
    }
    return -1;
}

string localteVex(AMGGraph* AMG, int vexId) {
    for (int i = 0; i < AMG->m_vexNum; i++) {
        if (AMG->m_vexName[i]->id == vexId) {
            return AMG->m_vexName[i]->name;
        }
    }
    return "no";
}

platform  getPlatform(vector<platform>  platform_1,string name) {
    platform  form;
    for (size_t i= 0; i < platform_1.size(); i++) {
        if (platform_1[i].name.compare(name)) {
            form = platform_1[i];
        }
    }
    return form;
}


vector<string> localteStatic(Atlas* atlas,int vexId) {
    AMGGraph* amg = atlas->AMG;
    string name =localteVex(amg, vexId);
    vector<string>  names = (amg->transfer)[name];
    return names;
}

vector<platform> localteStation(Atlas* atlas,string name) {
    Station* astion = atlas->station;
    vector<platform>  *names = astion->mymap[name];
    return *names;
}

void initTrip(Trip * trips) {
    trips->legs.clear();
    app.clear();
}

bool  ThreeStation(vector<string> vec ,string name) {
    bool  flags = false;
    for (size_t k = 0; k < vec.size(); k++) {
        if ((vec)[k].compare(name) == 0) {
            flags = true;
            break;
        }
    }
    return flags;
}

bool Onlineandoffline(Atlas* atlas,string lines,int j) {
    bool flags = false;
    vector<platform>  plmc = localteStation(atlas, lines);
    for (size_t k = 0; k < plmc.size(); k++) {
        if (plmc[k].name.compare(app[j]) == 0) {
            if (k == 0) {
                if (plmc[(k + 1)].name.compare(app[(j + 1)]) == 0) {
                    flags = true;
                    break;
                }
            }
            else if (k == (plmc.size() - 1)) {
                if (plmc[(k - 1)].name.compare(app[(j + 1)]) == 0) {
                    flags = true;
                    break;
                }
            }
            else {
                if ((plmc[(k - 1)].name.compare(app[(j + 1)]) == 0) || (plmc[(k + 1)].name.compare(app[(j + 1)])==0)) {
                    flags = true;
                    break;
                }
            }
        }
    }
    return flags;
}

void dijastral(Atlas* atlas, int start, int stop){

    AMGGraph* amg = atlas->AMG;
    Trip*  trips =atlas->trip;
    initTrip(trips);

    for (int i = 0; i < amg->m_vexNum; i++) {
         Distance[i] = amg->m_arcWeight[start][i];
         if (Distance[i] >-1  && Distance[i]!=  QID) {
             path[i] = start;
         }else {
             path[i] = -1;
         }
    }

    s[start] = true;
    Distance[start] = 0;
    int min = PI;
    int pos = 0; //
    for (int i = 1; i < amg->m_vexNum; i++) {
        min = PI;
        for (int j = 0; j < amg->m_vexNum; j++) {
          if (!s[j] && Distance[j] < min) {
                pos = j;
                min = Distance[j];
            }          
        }
        s[pos] = true;
        for (int k = 0; k < amg->m_vexNum; k++) {
            if (!s[k] && Distance[k] > amg->m_arcWeight[pos][k] + Distance[pos]) {
                int ace = amg->m_arcWeight[pos][k] + Distance[pos];
                Distance[k] = ace;
                path[k] = static_cast<int>(pos);
            }
        }
    }   
    //app.push_back(localteVex(amg, start));
    showPath(amg, start, stop);
    //±ê×¼µÏ½ÜÌØ

    if (app.size() == 0) {
        disDelete();
        throw std::runtime_error("No route.");
    }
    app.push_back(localteVex(amg, stop));
    trips->start = localteVex(amg, start);
    for (size_t i = 0 ; i <  app.size();  i++) {
        Trip::Leg  lgs;
        vector<string> vec =( amg->transfer)[app[i]];
        if (vec.size() == 1) {
            lgs.line = (vec)[0];
            bool flags = false;
            for (size_t j = (i+1); j < app.size(); j++) {
                vector<string> vec2 = (amg->transfer)[app[j]];
                if (vec2.size() > 1 &&  j!= (app.size()-1)) {
                    vector<string> vec3 = (amg->transfer)[app[(j + 1)]];
                    flags =ThreeStation(vec3, lgs.line);
                    if (flags) {
                        flags = Onlineandoffline(atlas, lgs.line, static_cast<int>(j));
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
            if (i == (app.size() - 1)) {
                lgs.stop = app[app.size() - 1];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
        else {
            size_t aps = i;
            bool flags = false;
            vector<string> vec2 = (amg->transfer)[app[(aps)]];
            for(size_t j = 0; j < vec.size(); j++){
                //for (size_t k = 0; k < vec2.size(); k++) {
                //    if (vec[j].compare(vec2[k]) == 0) {
                //        lgs.line = vec[j];
                //        flags = true;
                //        break;
                //    }
                //}
                //if (flags){
                //    flags = false;
                //    break;
                //}
                flags = Onlineandoffline(atlas, vec[j], static_cast<int>(aps));
                if (flags) {
                    lgs.line = vec[j];
                    flags = false;
                    break;
                }
            }

            for (size_t j = aps; j <app.size(); j++) {
                vec2 =(amg->transfer)[app[(j)]];
                if (vec2.size() > 1 && j != (size_t)(app.size() - 1)) {
                    vector<string> vec3 = (amg->transfer)[app[j + 1]];
                    flags = ThreeStation(vec3, lgs.line);
                    if (flags) {
                        flags = Onlineandoffline(atlas, lgs.line, static_cast<int>(j));
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
        //cout << vex << endl;
        app.push_back(vex);
    }
}

void disDelete() {   
     for (int i = 0; i < startSize; i++) {
         Distance[i] = 0;
         path[i] = 0;
         s[i] = false;
    }
}

void disDe() {
    delete [] Distance;
    delete [] path;
    delete [] s;
}
