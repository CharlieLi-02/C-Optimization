#include "Helpers.h"
#include "Atlas.h"
#include "Station.h"

// Space to implement helper class member functions.
// 用于实现帮助器类成员函数的空间。
// 地铁站是一个有向无环图


int Distance[100] = { 0 };
int path[100] = { 0 };
bool s[100] = { 0 };
vector<string>  app;

void Stringsplit(const string& str, const string& splits, vector<string>& res)
{
    if (str == "") {
        return;
    }
    //在字符串末尾也加入分隔符，方便截取最后一段
    string strs = str + splits;
    size_t pos = strs.find(splits);
    int step = (int)splits.size();
    // 若找不到内容则字符串搜索函数返回 npos
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + step, strs.size());
        pos = strs.find(splits);
    }
}


// 获取标记
int locateVex(AMGGraph *AMG, string vexName) {
    for (int i = 0; i < AMG->m_vexNum; i++) {
        if (AMG->m_vexName[i].name.compare(vexName) == 0) {
            return AMG->m_vexName[i].id;
        }
    }
    return -1;
}


//去掉首尾空格
void trim(string& s)
{
    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
}

// 获取名称
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
    for (int i= 0; i < platform_1.size(); i++) {
        if (platform_1[i].name.compare(name)) {
            form = platform_1[i];
        }
    }
    return form;
}

//线路规划
void  graphlines(Atlas* atlas, int start, int stop) {
    AMGGraph* amg = atlas->AMG;
    Station* station = atlas->station;
    map<int, vector<line_1>>  aline = station->lines;
    vector<string> startT = localteStatic(atlas, start);
    vector<string> stopT = localteStatic(atlas, stop);
    //初始单条
    //判断方向,判断换乘
    for (int i = 0;  i < startT.size();  i++) {
        for (int j = 0; j < stopT.size(); j++) {
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

//获取线路名称
vector<string> localteStatic(Atlas* atlas,int vexId) {
    AMGGraph* amg = atlas->AMG;
    string name =localteVex(amg, vexId);
    vector<string>  names = amg->transfer[name];
    return names;
}

// 获取线路站点
vector<platform> localteStation(Atlas* atlas,string name) {
    Station* astion = atlas->station;
    vector<platform>  names = astion->mymap[name];
    return names;
}

//判断方向（起始点 到终点 0 ，终点 到起始点方向 1 ），判断换乘线路
void initTrip(Trip * trips) {
    trips->legs.clear();
    app.clear();
}

// 下一站和当前站点共线路
bool  ThreeStation(vector<string> vec ,string name) {
    bool  flags = false;
    for (int k = 0; k < vec.size(); k++) {
        if (vec[k].compare(name) == 0) {
            flags = true;
            break;
        }
    }
    return flags;
}

//上线下线 不在同一条线路
bool Onlineandoffline(Atlas* atlas,string lines,int j) {
    bool flags = false;
    vector<platform>  plmc = localteStation(atlas, lines);
    for (int k = 0; k < plmc.size(); k++) {
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

    // 初始化
    initTrip(trips);
    for (int i = 0; i < amg->m_vexNum; i++) {
         Distance[i] = amg->m_arcWeight[start][i];
         if (Distance[i] > 0 && Distance[i]!= QID) {
             path[i] = start;
         }else {
             path[i] = -1;
         }
    }
    s[start] = true;
    Distance[start] = 0;
    int min = PI;
    int pos = 0; // 位置下标
    //初始化结束
    for (int i = 1; i < amg->m_vexNum; i++) {
        // 第一次循环
        min = PI;
        for (int j = 0; j < amg->m_vexNum; j++) {
         if (!s[j] && Distance[j] < min) {
                pos = j;
                min = Distance[j];  //获取最小点信息
            }
           
        }
        s[pos] = true;
        for (int k = 0; k < amg->m_vexNum; k++) {
            if (!s[k] && Distance[k] > amg->m_arcWeight[pos][k] + Distance[pos]) {
                //cout << "Distance" << Distance[pos]<<endl;
                //cout << "m_arcWeight" << amg->m_arcWeight[pos][k] << endl;
                int ace = amg->m_arcWeight[pos][k] + Distance[pos];
                //cout <<"距离" << ace << endl;
                Distance[k] = ace;
                path[k] = pos;
            }
        }
    }   
    showPath(amg, start, stop);
    if (app.size() == 0) {
        disDelete();
        std::cout << "-------完成--------" << endl;
        return;
    }
    cout << "终点站:" << localteVex(amg, stop) << endl;
    app.push_back(localteVex(amg, stop));
    //确认起始站
    trips->start = localteVex(amg, start);
    //线路---
    for (int i = 0 ; i <  app.size();  i++) {
        Trip::Leg  lgs;
        vector<string> vec = amg->transfer[app[i]];
        if (vec.size() == 1) {
            // 无换乘情况
            lgs.line = vec[0];
            bool flags = false;
            for (auto j = (i+1); j < app.size(); j++) {
                vector<string> vec2 = amg->transfer[app[j]];
                if (vec2.size() > 1 &&  j!= (app.size()-1)) {
                    //第三个站点和第一个站点不在一条线上，换乘                   
                    vector<string> vec3 = amg->transfer[app[(j + 1)]];
                    flags =ThreeStation(vec3, lgs.line);
                    //沿线查询
                    if (flags) {
                        //上线下线 不在同一条线路
                        flags = Onlineandoffline(atlas, lgs.line, j);
                    }
                    if (!flags) {
                        //发生换乘
                        lgs.stop = app[j];
                        flags = false;
                        break;
                    }
                    flags = false;
                }
                i = j;
            }
            if (i == (app.size() - 1)) {
                //到达终点站下车
                lgs.stop = app[app.size() - 1];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
        else {
            //假如在换乘站
            int aps = i + 1;
            bool flags = false;
            //获取线路
            vector<string> vec2 = amg->transfer[app[(aps)]];
            for (int j = 0; j < vec.size(); j++) {
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
                    //第三个站点和第一个站点不在一条线上，换乘
                    flags = ThreeStation(vec3, lgs.line);
                    if (flags) {
                        //上线下线 不在同一条线路
                        flags = Onlineandoffline(atlas, lgs.line, j);
                    }
                    if (!flags) {
                        //发生换乘
                        lgs.stop = app[j];
                        flags = false;
                        break;
                    }
                    flags = false;
                }
                i = j;
            }
            if (i==(app.size()-1)) {
                //到达终点站下车
                lgs.stop = app[app.size() - 1];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
    }
    std::cout << "消耗的时间:" << Distance[stop]<<endl;
    disDelete();
    std::cout << "-------完成--------" << endl;
}


void showPath(AMGGraph *AMG, int startVexAdd, int endVexAdd) {
    if (path[endVexAdd] != -1) {
        showPath(AMG, startVexAdd, path[endVexAdd]);
        string vex = localteVex(AMG, path[endVexAdd]);
        //AMG->transfer[vex]
        app.push_back(vex);
        cout << "沿途经过:" << localteVex(AMG, path[endVexAdd]) << endl;
        
    }
}

void disDelete() {   
     for (int i = 0; i < 100; i++) {
         Distance[i] = 0;
         path[i] = 0;
         s[i] = false;
     }
}