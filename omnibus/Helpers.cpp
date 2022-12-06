#include "Helpers.h"
#include "Atlas.h"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
// Space to implement helper class member functions.



vector<string>  app;

int locateVex(AMGGraph* AMG, string vexName) {
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

platform  getPlatform(vector<platform>  platform_1, string name) {
    platform  form;
    for (size_t i = 0; i < platform_1.size(); i++) {
        if (platform_1[i].name.compare(name)) {
            form = platform_1[i];
        }
    }
    return form;
}


vector<string> localteStatic(Atlas* atlas, int vexId) {
    AMGGraph* amg = atlas->AMG;
    string name = localteVex(amg, vexId);
    vector<string>  names = (amg->transfer)[name];
    return names;
}

vector<platform> localteStation(Atlas* atlas, string name) {
    Station* astion = atlas->station;
    vector<platform>* names = astion->mymap[name];
    return *names;
}

void initTrip(Trip* trips) {
    trips->legs.clear();
    app.clear();
}

bool  ThreeStation(vector<string> vec, string name) {
    bool  flags = false;
    for (size_t k = 0; k < vec.size(); k++) {
        if ((vec)[k].compare(name) == 0) {
            flags = true;
            break;
        }
    }
    return flags;
}

bool Onlineandoffline(Atlas* atlas, string lines, int j) {
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
                if ((plmc[(k - 1)].name.compare(app[(j + 1)]) == 0) || (plmc[(k + 1)].name.compare(app[(j + 1)]) == 0)) {
                    flags = true;
                    break;
                }
            }
        }
    }
    return flags;
}


void  intDijkstra(Atlas* atlas, int start, int stop) {
    int* path = (int*)malloc(sizeof(int) * atlas->G->numNodes);
    int* dist = (int*)malloc(sizeof(int) * atlas->G->numNodes);    
    AMGGraph* amg = atlas->AMG;
    Trip* trips = atlas->trip;
    initTrip(trips);
    Dijkstra(atlas, dist, path, start, stop);
    free(path);
    free(dist);
    if (app.size() == 1) {
        throw std::runtime_error("No route.");
    }
    trips->start = localteVex(amg, start);
    for (size_t i = 0; i < app.size(); i++) {
        Trip::Leg  lgs;
        vector<string> vec = (amg->transfer)[app[i]];
        if (vec.size() == 1) {
            lgs.line = (vec)[0];
            bool flags = false;
            for (size_t j = (i + 1); j < app.size(); j++) {
                vector<string> vec2 = (amg->transfer)[app[j]];
                if (vec2.size() > 1 && j != (app.size() - 1)) {
                    vector<string> vec3 = (amg->transfer)[app[(j + 1)]];
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
            for (size_t j = 0; j < vec.size(); j++) {
                flags = Onlineandoffline(atlas, vec[j], static_cast<int>(aps));
                if (flags) {
                    lgs.line = vec[j];
                    flags = false;
                    break;
                }
            }

            for (size_t j = aps; j < app.size(); j++) {
                vec2 = (amg->transfer)[app[(j)]];
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
            if (i == (app.size() - 1)) {
                lgs.stop = app[app.size() - 1];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
    }
    
}


// 构建邻接表 矩阵
void Dijkstra(Atlas* AT, int dist[], int path[], int v, int stop)
{
    AGraph* G = AT->G;
    int set[MAXVEX];
    int i, j, u = 0, min = 0;
    //给三个数组赋初值
    for (i = 0; i < G->numNodes; i++)
    {
        set[i] = 0;
        path[i] = -1;
        dist[i] = INF;
    }

    ArcNode* p = G->adjlist[v].firstarc;
    while (p != NULL)
    {
        dist[p->adjvex] = p->weight;
        path[p->adjvex] = v;
        p = p->nextarc;
    }
    path[v] = -1;
    set[v] = 1;
    dist[v] = 0;

    for (i = 1; i < G->numNodes; i++)
    {
        min = INF;

        for (j = 0; j < G->numNodes; j++)
        {
            if (set[j] == 0 && dist[j] < min)
            {
                u = j;
                min = dist[j];             
            }
        }
        set[u] = 1;
        for (j = 0; j < G->numNodes; j++)
        {
            int weight = getWeight(G, u, j);
            if (set[j] == 0 && dist[u] + weight < dist[j])
            {
                dist[j] = dist[u] + weight;
                path[j] = u;
            }
        }
    }
    for (int i = 0; i < G->numNodes; i++) {
        cout << path[i] << endl;
    }
    print_path(AT->AMG, path, stop);
    //DFSPrint(AT->AMG, v, stop, path);
}

//获得边的权重
int getWeight(AGraph* G, int u, int j)
{
    if (u == j) {
        return 0;
    }   
    ArcNode* p = G->adjlist[u].firstarc;
    while (p != NULL)
    {
        if (p->adjvex == j) {
            return p->weight;
        }        
        p = p->nextarc;
    }
    return INF;
}

//创建图
void CreateGraph(AGraph* G, Atlas* atlas)
{
    int i;
    map<string, vector<platform>*>  psm = atlas->station->mymap;
    G->numNodes = atlas->AMG->m_vexNum;
    for (i = 0; i < G->numNodes; i++)
    {
        G->adjlist[i].firstarc = NULL;
    }
    short vName_id = 0;
    for (auto oc = psm.begin(); oc != psm.end(); oc++)
    {
        vector<platform>* plm = oc->second;
        if (plm == NULL) {
            continue;
        }
        int gid = -1; // 当前对象标量  
        platform  original = (*plm->begin());
        for (auto ac = plm->begin(); ac != plm->end(); ac++)
        {
            vexName* vName = NULL;
            vector<vexName*>::iterator v_N = find_if(atlas->AMG->m_vexName.begin(), atlas->AMG->m_vexName.end(), finder_t(ac->name));
            if (v_N != atlas->AMG->m_vexName.end()) {
                gid = (*v_N)->id;
            }
            else {
                vName = new vexName;
                vName->name = ac->name;
                vName->id = vName_id++;
                atlas->AMG->m_vexName.push_back(vName);
            }
            if (original.name.compare(ac->name) != 0) {

                ArcNode* pe = (ArcNode*)malloc(sizeof(ArcNode));
                ArcNode* reverse = (ArcNode*)malloc(sizeof(ArcNode));
                vector<vexName*>::iterator v_N2 = find_if(atlas->AMG->m_vexName.begin(), atlas->AMG->m_vexName.end(), finder_t(original.name));               
                if (gid != -1) {
                    pe->adjvex = gid;
                    //当前对象有重复
                    reverse->adjvex = (*v_N2)->id;
                    reverse->nextarc = G->adjlist[gid].firstarc;
                    G->adjlist[gid].firstarc = reverse;
                    pe->nextarc = G->adjlist[(*v_N2)->id].firstarc;
                    G->adjlist[(*v_N2)->id].firstarc = pe;
                    gid = -1;
                }else {
                    // 当前对象无重复                   
                    reverse->adjvex = (*v_N2)->id;
                    reverse->nextarc = G->adjlist[vName_id - 1].firstarc;
                    G->adjlist[vName_id - 1].firstarc = reverse;
                    pe->adjvex = vName_id-1;
                    pe->nextarc = G->adjlist[(*v_N2)->id].firstarc;
                    G->adjlist[(*v_N2)->id].firstarc = pe;
                }
                
                pe->weight = (int)((ac->timer) - original.timer);
                reverse->weight = (int)((ac->timer) - original.timer);
            }
            gid = -1;
            original = (*ac);
        }
        int  m_arcNum = int(plm->size() - 1);
        atlas->AMG->m_arcNum = atlas->AMG->m_arcNum + m_arcNum;
    }
    G->numEdges = atlas->AMG->m_arcNum;
}


//输出路径
void print_path(AMGGraph* AMG, int path[], int v1)
{
    stack<int> st;
    st.push(v1);

    while (!st.empty()) {
        if (path[v1] == -1) {
            while (!st.empty()) {
                v1 = st.top();
                string vex = localteVex(AMG, v1);
                app.push_back(vex);
                //cout << vex << endl;
                st.pop();
            }
        }
        else
        {
            v1 = path[v1];
            st.push(v1);
        }

    }
}

//输出从起点s到顶点v的最短路径
void DFSPrint(AMGGraph* AMG, int s, int v, int path[])
{
    if (v == s)
    {
        cout << s << " ";
        string vex = localteVex(AMG,s);
        app.push_back(vex);
        cout << vex << endl;
        return;
    }
    else {
        DFSPrint(AMG, s, path[v], path);
        string vex = localteVex(AMG, v);
        app.push_back(vex);
        cout << vex << endl;
    }

}


