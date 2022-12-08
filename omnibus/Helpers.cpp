#include "Helpers.h"
#include "Atlas.h"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>

// Space to implement helper class member functions.

vector<string>  app;

vector<string> split(string str, string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
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

//打印路径信息 
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
                    //flags = Onlineandoffline(atlas, lgs.line, static_cast<int>(j));
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
                    //flags = Onlineandoffline(atlas, lgs.line, static_cast<int>(j));
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
    print_path(AT->AMG, path, stop);

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

//创建邻接表
 void CreateGraph(AGraph* G, Atlas* atlas)
{
    map<string, vector<platform>*>  psm = atlas->station->mymap;
    G->numNodes = atlas->AMG->m_vexNum;
    int vName_id = 0;
    G->adjlist = new VNode[G->numNodes];
    for (int i = 0; i < G->numNodes; i++)
    {
        G->adjlist[i].firstarc = NULL;
    }
    int temp = 0;
    for (auto oc = psm.begin(); oc != psm.end(); oc++)
    {
        vector<platform>* plm = oc->second;
        platform  original = (*plm->begin());
        for (auto ac = plm->begin(); ac != plm->end(); ac++) {
            string  acName = oc->first + "/" + ac-> name;
            int next = locateVex(atlas->AMG, acName);
            if (next == -1) {
                 continue;
            }
            if (original.name.compare(ac->name) != 0) {
                string  acNameB = oc->first + "/" + original.name;
                int prev = locateVex(atlas->AMG, acNameB);
                if (prev == -1) {
                     continue;
                }
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
    for (auto acp = atlas->AMG->transfer.begin(); acp != atlas->AMG->transfer.end(); acp++) {
        if (acp->second.size() > 1) {
            atlas->AMG->m_arcNum = int(atlas->AMG->m_arcNum +acp->second.size() - 1);
            string original = acp->second[0] + "/" + acp->first;
            for (int j = 0; j < acp->second.size(); j++) {
                string  acNameB = acp->second[j] + "/" + acp->first;
                int next = locateVex(atlas->AMG, original);
                if (next == -1) {
                    continue;
                }
                if (acNameB.compare(original) != 0){                    
                    int prev = locateVex(atlas->AMG, acNameB);
                    if (prev == -1) {
                        continue;
                    }
                    ArcNode* pe = (ArcNode*)malloc(sizeof(ArcNode));
                    ArcNode* reverse = (ArcNode*)malloc(sizeof(ArcNode));
                    if (pe != NULL && reverse != NULL) {
                        reverse->adjvex = next;
                        reverse->nextarc = G->adjlist[prev].firstarc;
                        G->adjlist[prev].firstarc = reverse;
                        pe->adjvex = prev;
                        pe->nextarc = G->adjlist[next].firstarc;
                        G->adjlist[next].firstarc = pe;                      
                        pe->weight = 0;
                        reverse->weight = 0;
                    }
                }
                original = acNameB;
            }
        }       
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





int visited[MAXVEX] = { 0 };
priority_queue<Node>q;
int index =-1;
int counts = 0;
void Dijkstra2(Atlas* AT, int v, int stop) {
    //chrono::milliseconds stop1 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    int *path2= new int[AT->AMG->m_vexNum];
    Node* node2 = new Node[AT->AMG->m_vexNum];
    Dijkstra(AT, v, AT->AMG->m_vexNum,path2,node2);
    AT->AMG->node.insert(pair<int, Node*>(v, node2));
    AT->AMG->path.insert(pair<int, int*>(v, path2));
    int temp = stop;
    //app.push_back(localteVex(AT->AMG, temp));
    int cossu = 0;
    while (path2[temp] != -1) {
         //app.push_back(localteVex(AT->AMG, path2[temp]));
         //cout << localteVex(AT->AMG, path2[temp])<<endl;
         cossu = cossu + node2[temp].w;
         temp = path2[temp];        
    } 
    if (index == -1) {
        index = v;
    }
    else {
        if (cossu < counts) {
            index = v;
            counts = cossu;
        }
    }  
    //if (app.size() == 1) {
    //    throw std::runtime_error("No route.");
    //}
    //reverse(app.begin(), app.end());
    //chrono::milliseconds stop2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "查询时间" << (stop2.count() - stop1.count()) << endl;
    //print_line(AT, v);
    //chrono::milliseconds stop3 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    //cout << "打印时间" << (stop3.count() - stop2.count()) << endl;
}

void  print_line_2(Atlas* AT, int path[],int stop) {
    Trip* trips = AT->trip;
    initTrip(trips);
    app.push_back(localteVex(AT->AMG, stop));
    while (path[stop] != -1) {
        app.push_back(localteVex(AT->AMG, path[stop]));
        stop = path[stop];
    }
    if (app.size() == 1) {
        throw std::runtime_error("No route.");
    }
    reverse(app.begin(), app.end());
    unordered_map<string, vector<string>> cpps;
    bool  flags_1 = true;
    int  s_name_flag = 0;
    string alam=  split(app[0], "/")[0] +";" + to_string(s_name_flag);
    for (int i = 0; i < app.size(); i++) {
           vector<string> name = split(app[i], "/");
           string name_v = name[0] + ";" + to_string(s_name_flag);
           if (alam.compare(name_v) == 0) {
                alam = name[0]+";"+ to_string(s_name_flag);
                unordered_map<string, vector<string>>::iterator as = cpps.find(name_v);
                if (as != cpps.end()) {
                    as->second.push_back(name[1]);
                }
                else {
                    vector<string>  line_name;
                    line_name.push_back(name[1]);
                    cpps.insert(pair<string, vector<string>>(name_v, line_name));
                }
           }
           else {
               s_name_flag++;
               string name_v_1 = name[0] + ";" + to_string(s_name_flag);
               unordered_map<string, vector<string>>::iterator as = cpps.find(name_v_1);
               if (as != cpps.end()) {
                   as->second.push_back(name[1]);
               }
               else {
                   vector<string>  line_name;
                   line_name.push_back(name[1]);
                   cpps.insert(pair<string, vector<string>>(name_v_1, line_name));
               }
               alam = name_v_1;
           }
    }
    int  aps = 0;
    for (auto oc = cpps.begin(); oc != cpps.end();oc++)
    {
        if (oc->second.size() == 1) {
            continue;
        }
        Trip::Leg  lgs;
        if (aps == 0) {
            trips->start = oc->second[0];
            lgs.line = split(oc->first,";")[0];
            lgs.stop = oc->second[oc->second.size() - 1];
            aps++;
            trips->legs.push_back(lgs);
            continue;
        }
        else {
            lgs.line = split(oc->first,";")[0];
            lgs.stop = oc->second[oc->second.size() - 1];
            trips->legs.push_back(lgs);
        }
        aps++;     
    }
}


inline void  Dijkstra(Atlas* AT, int v0, int n,int path2[],Node dist2[])
{
    AGraph* g = AT->G;
    for (int i = 0; i < n; i++)
    {
        dist2[i].id = i;     //从源v0到结点i
        dist2[i].w = INF;    //初始距离为INF
        path2[i] = -1;       //初始最短路径的前一个结点为-1
        visited[i] = 0;     //初始为所有结点都没有访问过
    }
    dist2[v0].w = 0;
    q.push(dist2[v0]);       //将源点入优先队列
    vector<vector<int> > prevPoints;
    while (!q.empty())
    {
        Node node = q.top();  //取得当前权值w最小值对应的结点
        q.pop();
        int u = node.id;
        if (visited[u]) {
            continue;
        }     //若当前结点还没有访问过         
        visited[u] = 1;
        ArcNode* p = g->adjlist[u].firstarc;    //取得当前结点对应的第一条边

        while (p)
        {
            int tempv = p->adjvex;
            int tempw = p->weight;
            //cout << "站点" << localteVex(AT->AMG, p->adjvex) << "权重" << tempw << endl;
            //若该边对应的另外一个结点还没有访问过并且源点到u加上u到tempv的权值小于源点直接到tempv的权值，更新权值
            if (!visited[tempv] && dist2[tempv].w >dist2[u].w + tempw)
            {
                dist2[tempv].w = dist2[u].w + tempw;              
                path2[tempv] = u;        // 更新源点到tempv的最短路径的tempv前一结点为u
                //cout << "站点" << localteVex(AT->AMG, p->adjvex) << "权重" << dist2[tempv].w << endl;
                q.push(dist2[tempv]);
            }
            p = p->nextarc;
        }
    }
    //for (int i = 0; i < n; i++) {
    //    cout << "测试-1" << path2[i] << endl;
    //    cout << "测试" << dist2[i].w << endl;
    //}
}


/**
  *
  * @param start 起点
  * @param dest 终点
  * @param adj 邻接链表。adj[i][k]表示节点i的第k个邻接点的索引
  * @param distance 到起点的距离。distance[i]表示起点到点i的距离
  * @return prevPoints 前驱点数组。 prevPoints[k]表示到达点k的最短路径中的所有前驱点
  */

//vector<vector<int> > dijkstra3(Atlas* AT,int start,int dest = -2,vector<int> distance) {
//    int num = AT->G->numNodes;
//    vector<bool> visited(num, false);
//    visited[start] = true;
//
//    vector<vector<int> > prevPoints;
//
//    //前驱点数组初始化
//    for (int i = 0; i < num; ++i) {
//        if (distance[i] <INF) {
//            prevPoints.push_back(vector<int>(1, start));
//        }
//        else {
//            prevPoints.push_back(vector<int>(1, -1));
//        }
//    }
//
//    if (prevPoints[dest][0] == start) {
//        return prevPoints;
//    }
//
//    for (int i = 1; i < num; ++i) {
//        // 找离起点最近的点
//        // 这里的复杂度还是O(N)，可以通过使用优先队列优化
//        int closest = INF;
//        int toVisit = -1;
//        for (int j = 0; j < num; ++j) {
//            if (!visited[j] && distance[j] < closest) {
//                closest = distance[j];
//                toVisit = j;
//            }
//        }
//
//        //如果是要找指定终点dest，可以提前剪枝，
//        //但这样的话未访问的点的路径就不能保证是最短的。
//        if (toVisit != -1 && !(dest != -2 && toVisit == dest)) {
//            //更新最短路径
//            visited[toVisit] = true;
//            for (int k = 0; k < adj[toVisit].size(); k++) {
//                if (adj[toVisit][k] != -1 && !visited[adj[toVisit][k]]) {
//                    if (distance[adj[toVisit][k]] > distance[toVisit] + 1) {
//                        //update the distance
//                        distance[adj[toVisit][k]] = distance[toVisit] + 1;
//                        //clear the paths,and store the new path
//                        prevPoints[adj[toVisit][k]].clear();
//                        prevPoints[adj[toVisit][k]].push_back(toVisit);
//
//                    }
//                    else if (distance[adj[toVisit][k]] == distance[toVisit] + 1) {
//                        //add the new path
//                        prevPoints[adj[toVisit][k]].push_back(toVisit);
//                    }
//                }
//            }
//        }
//        else {
//            break;
//        }
//    }
//    return prevPoints;
//}