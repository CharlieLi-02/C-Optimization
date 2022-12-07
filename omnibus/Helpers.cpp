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
    map<string, int>::iterator as = AMG->m_vexId.find(vexName);
    if (as != AMG->m_vexId.end()) {
        return  as->second;
    }
    return -1;
}

string localteVex(AMGGraph* AMG, int vexId) {
    map<int, string>::iterator as =AMG->m_vexName.find(vexId);
    if (as != AMG->m_vexName.end()) {
        return  as->second;
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

vector<platform>* localteStation(Atlas* atlas, string name) {
    return atlas->station->mymap[name];
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
    vector<platform>  *plmc = localteStation(atlas, lines);
    for (size_t k = 0; k <(*plmc).size(); k++) {
        if ((*plmc)[k].name.compare(app[j]) == 0) {
            if(k == 0) {
                if ((*plmc)[(k + 1)].name.compare(app[j+1]) == 0) {
                    flags = true;
                    break;
                }
            }
            else if (k == ((*plmc).size() - 1)) {
                if ((*plmc)[(k - 1)].name.compare(app[j+1]) == 0) {
                    flags = true;
                    break;
                }
            }
            else {
                if (((*plmc)[(k - 1)].name.compare(app[j+1]) == 0) || ((*plmc)[(k + 1)].name.compare(app[j+1]) == 0)) {
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
    print_line(atlas, start);
}

//��ӡ·����Ϣ 
void  print_line(Atlas* atlas, int start) {
    AMGGraph* amg = atlas->AMG;
    Trip* trips = atlas->trip;
    trips->start = localteVex(amg, start);
    size_t tempSize = app.size() - 1;
    for (size_t i = 0; i < app.size(); i++) {
        Trip::Leg  lgs;
        vector<string> vec = (amg->transfer)[app[i]];
        if (vec.size() == 1) {
            lgs.line = (vec)[0];
            bool flags = false;
            for (size_t j = (i + 1); j < app.size(); j++) {
                vector<string> vec2 = (amg->transfer)[app[j]];
                if (vec2.size() > 1 && j != tempSize) {
                    vector<string> vec3 = (amg->transfer)[app[(j + 1)]];
                    flags = ThreeStation(vec3, lgs.line);
                    if (flags) {
                        flags = Onlineandoffline(atlas, lgs.line, static_cast<int>(j));
                    }
                    if (!flags) {
                        lgs.stop = app[j];
                        flags = false;
                        //i = j;
                        break;
                    }
                    flags = false;
                }
                i = j;
            }
            if (i == tempSize) {
                lgs.stop = app[tempSize];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
        else {
            size_t aps = i;
            bool flags = false;
            vector<string> vec2 = (amg->transfer)[app[(aps)]];
            if (vec2.size() == 1) {
                lgs.line = vec2[0];
            }else {
                for (size_t j = 0; j < vec.size(); j++) {
                    flags = Onlineandoffline(atlas, vec[j], static_cast<int>(aps));
                    if (flags) {
                        lgs.line = vec[j];
                        flags = false;
                        break;
                    }
                }
            }
            for (size_t j = aps; j < app.size(); j++) {
                if (vec2.size() > 1 && j != tempSize) {
                    vector<string> vec3 = (amg->transfer)[app[j + 1]];
                    flags = ThreeStation(vec3, lgs.line);
                    if (flags) {
                        flags = Onlineandoffline(atlas, lgs.line, static_cast<int>(j));
                    }
                    if (!flags) {
                        lgs.stop = app[j];
                        flags = false;
                        //i = j;
                        break;
                    }
                    flags = false;
                }
                i = j;
            }
            if (i == tempSize) {
                lgs.stop = app[tempSize];
                trips->legs.push_back(lgs);
                break;
            }
            trips->legs.push_back(lgs);
        }
    }
}


// �����ڽӱ� ����
void Dijkstra(Atlas* AT, int dist[], int path[], int v, int stop)
{
    AGraph* G = AT->G;
    int set[MAXVEX];
    int i, j, u = 0, min = 0;
    //���������鸳��ֵ
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
    print_path(AT->AMG, path, stop);

}

//��ñߵ�Ȩ��
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

//�����ڽӱ�
 void CreateGraph(AGraph* G, Atlas* atlas)
{
    map<string, vector<platform>*>  psm = atlas->station->mymap;
    G->numNodes = atlas->AMG->m_vexNum;
    short vName_id = 0;
    G->adjlist = new VNode[G->numNodes];
    for (int i = 0; i < G->numNodes; i++)
    {
        G->adjlist[i].firstarc = NULL;
    }
    for(auto acp = atlas->AMG->transfer.begin(); acp != atlas->AMG->transfer.end(); acp++) {        
        atlas->AMG->m_vexName.insert(pair<int, string >(vName_id, acp->first));
        atlas->AMG->m_vexId.insert(pair<string, int>(acp->first, vName_id));     
        vName_id++;
    }
    int temp = 0;
    for (auto oc = psm.begin(); oc != psm.end(); oc++)
    {
        vector<platform>* plm = oc->second;
        platform  original = (*plm->begin());
        for (auto ac = plm->begin(); ac != plm->end(); ac++) {
            int next = locateVex(atlas->AMG, ac->name);
            if (original.name.compare(ac->name) != 0) {
                int prev = locateVex(atlas->AMG, original.name);
                ArcNode* pe = (ArcNode*)malloc(sizeof(ArcNode));
                ArcNode* reverse = (ArcNode*)malloc(sizeof(ArcNode));
                if (pe != NULL && reverse != NULL) {
                    reverse->adjvex = next;
                    reverse->nextarc = G->adjlist[prev].firstarc;
                    G->adjlist[prev].firstarc = reverse;
                    pe->adjvex = prev;
                    pe->nextarc = G->adjlist[next].firstarc;
                    G->adjlist[next].firstarc = pe;
                    temp =((ac->timer) - original.timer);
                    pe->weight = temp;
                    reverse->weight = temp;
                }
            }
            original = (*ac);
        }
        int  m_arcNum = int((plm->size() - 1)*2);
        atlas->AMG->m_arcNum = atlas->AMG->m_arcNum + m_arcNum;
    }
    G->numEdges = atlas->AMG->m_arcNum;
}


//���·��
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

//��������s������v�����·��
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




Node dist2[MAXVEX];
int visited[MAXVEX] = { 0 };
priority_queue<Node>q;
void Dijkstra2(Atlas* AT, int v, int stop) {
    //chrono::milliseconds stop1 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    Trip* trips = AT->trip;
    initTrip(trips);
    int *path2= new int[AT->AMG->m_vexNum];
    Dijkstra(AT, v, AT->AMG->m_vexNum,path2);
    int temp = stop;
    app.push_back(localteVex(AT->AMG, temp));
    while (path2[temp] != -1) {
         app.push_back(localteVex(AT->AMG, path2[temp]));
         //cout << localteVex(AT->AMG, path2[temp])<<endl;
         temp = path2[temp];
    }
    AT->AMG->path.insert(pair<int, int*>(v, path2));
    if (app.size() == 1) {
        throw std::runtime_error("No route.");
    }
    reverse(app.begin(), app.end());     
    chrono::milliseconds stop2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "��ѯʱ��" << (stop2.count() - stop1.count()) << endl;
    print_line(AT, v);
    chrono::milliseconds stop3 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    cout << "��ӡʱ��" << (stop3.count() - stop2.count()) << endl;
}




inline void  Dijkstra(Atlas* AT, int v0, int n,int path2[])
{
    AGraph* g = AT->G;
    for (int i = 0; i < n; i++)
    {
        dist2[i].id = i;     //��Դv0�����i
        dist2[i].w = INF;    //��ʼ����ΪINF
        path2[i] = -1;       //��ʼ���·����ǰһ�����Ϊ-1
        visited[i] = 0;     //��ʼΪ���н�㶼û�з��ʹ�
    }
    dist2[v0].w = 0;
    q.push(dist2[v0]);       //��Դ�������ȶ���

    while (!q.empty())
    {
        Node node = q.top();  //ȡ�õ�ǰȨֵw��Сֵ��Ӧ�Ľ��
        q.pop();
        int u = node.id;
        if (visited[u]) {
            continue;
        }     //����ǰ��㻹û�з��ʹ�         
        visited[u] = 1;
        ArcNode* p = g->adjlist[u].firstarc;    //ȡ�õ�ǰ����Ӧ�ĵ�һ����

        while (p)
        {
            int tempv = p->adjvex;
            int tempw = p->weight;
            //���ñ߶�Ӧ������һ����㻹û�з��ʹ�����Դ�㵽u����u��tempv��ȨֵС��Դ��ֱ�ӵ�tempv��Ȩֵ������Ȩֵ
            if (!visited[tempv] && dist2[tempv].w >=dist2[u].w + tempw)
            {
                dist2[tempv].w = dist2[u].w + tempw;
                path2[tempv] = u;        // ����Դ�㵽tempv�����·����tempvǰһ���Ϊu
                q.push(dist2[tempv]);
            }
            p = p->nextarc;
        }
    }
}