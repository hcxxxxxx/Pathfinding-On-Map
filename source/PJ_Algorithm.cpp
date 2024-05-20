#include "PJ.h"

int Euclid(ll start_id, ll end_id)//Calculate Euclid distance
{
    return pow(nodesinsight[start_id].x - nodesinsight[end_id].x, 2) + pow(nodesinsight[start_id].y - nodesinsight[end_id].y, 2);
}

int Manhattan(ll start_id, ll end_id)//Calculate Manhattan distance
{
    return abs(nodesinsight[start_id].x - nodesinsight[end_id].x) + abs(nodesinsight[start_id].y - nodesinsight[end_id].y);
}

void Closest(int n)//Find closest node according to the click point
{
    int mindis = 10000000;
    int dis;
    for (auto& pair : nodesinsight)//Find closest node using Manhattan distance
    {
        if (graph[pair.first].empty()) continue;
        if (n == 1) dis = abs(m_x1 - pair.second.x) + abs(m_y1 - pair.second.y);
        else dis = abs(m_x2 - pair.second.x) + abs(m_y2 - pair.second.y);
        if (dis < mindis)
        {
            mindis = dis;
            if (n == 1) n_1 = pair.second;
            else n_2 = pair.second;
        }
    }
}

void Dijkstra(bool demonstrate)//Dijkstra algorithm
{
    setlinecolor(RED);
    setlinestyle(PS_SOLID, 3 + 0.5 / ratio);
    ll start_id = n_1.id;//Dijkstra starting node
    ll end_id = n_2.id;//Dijkstra ending node
    int n = graph.size();
    const int INF = 0x7ffffff;
    unordered_map<ll, int> dist;//Distance
    unordered_map<ll, ll> prev;//Previous node
    for (auto& pair : graph)//Initialization
    {
        ll id = pair.first;
        dist[id] = INF;
    }
    dist[start_id] = 0;
    priority_queue<pair<int, ll>, vector<pair<int, ll>>, greater<pair<int, ll>>> pq;//Build priority queue
    pq.push({ 0, start_id });
    setfillcolor(COLOR_DIJ_TRAVERSED_NODES);
    while (!pq.empty())
    {
        int d = pq.top().first;
        ll u = pq.top().second;
        pq.pop();
        int tempx = nodes[u].x;
        int tempy = nodes[u].y;
        if (u == end_id) break;

        if ((!(tempx >= ov_left && tempx <= ov_right && tempy >= ov_top && tempy <= ov_bottom)) && demonstrate) solidcircle(tempx, tempy, 1 + 0.25 / ratio);
        if (d > dist[u]) continue;
        for (const auto& e : graph[u])
        {
            ll v = e.to;
            int w = e.weight;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
                prev[v] = u;
                if (u != 0 && v == end_id) goto A;
            }
        }
    }
    A:
    ll s1 = end_id;
    ll s2 = 0;
    int s1x = -1, s1y = -1, s2x = -1, s2y = -1;
    int cnt = 0;
    while (s1 != start_id)//Reverse
    {
        s2 = prev[s1];
        if (s2 == 0)
        {
            cout << "start_id: " << start_id << " end_id: " << end_id << endl;
            cout << "s1: " << s1 << endl;
            cout << "prev[s1]: " << prev[s1] << endl;
            cout << "Please choose again!!!" << endl;
            break;
        }
        s1x = nodes[s1].x;
        s1y = nodes[s1].y;
        s2x = nodes[s2].x;
        s2y = nodes[s2].y;
        if (!(((s1x >= ov_left && s1x <= ov_right) && (s1y >= ov_top && s1y <= ov_bottom)) || ((s2x >= ov_left && s2x <= ov_right) && (s2y >= ov_top && s2y <= ov_bottom))))
        {
            line(s2x, s2y, s1x, s1y);
            if (demonstrate) Sleep(10);//Demonstrate algorithm process
        }
        s1 = s2;
    }
    setlinestyle(PS_SOLID, linewidth);
}

void Astar(bool demonstrate)//Astar algorithm
{
    setlinecolor(COLOR_ASTAR_ROAD);
    setlinestyle(PS_SOLID, 3 + 0.5 / ratio);
    ll start_id = n_1.id;//Astar starting node
    ll end_id = n_2.id;//Astar ending node
    int n = graph.size();
    const int INF = 0x7fffffff;
    priority_queue<pair<int, ll>, vector<pair<int, ll>>, greater<pair<int, ll>>> pq;//Build priority queue
    unordered_map<ll, int> estimated_dis, actual_dis;//Estimated distance & Actual distance
    unordered_map<ll, ll> prev;//Previous node
    for (auto& pair : graph)//Initialization
    {
        ll id = pair.first;
        estimated_dis[id] = INF;
        actual_dis[id] = INF;
    }
    estimated_dis[start_id] = Manhattan(start_id, end_id);
    actual_dis[start_id] = 0;
    pq.push({ estimated_dis[start_id],start_id });
    setfillcolor(COLOR_ASTAR_TRAVERSED_NODES);
    while (!pq.empty())
    {
        ll u = pq.top().second;
        pq.pop();
        int tempx = nodesinsight[u].x;
        int tempy = nodesinsight[u].y;

        if (u == end_id)//Shortest path found
        {
            break;
        }

        if ((!(tempx >= ov_left && tempx <= ov_right && tempy >= ov_top && tempy <= ov_bottom)) && demonstrate) solidcircle(tempx, tempy, 1 + 0.25 / ratio);//展示模式：显示遍历过的点

        for (const auto& e : graph[u])
        {
            ll v = e.to;
            if (v == end_id) prev[v] = u;
            int w = e.weight;
            if (actual_dis[u] + w < actual_dis[v])
            {
                actual_dis[v] = actual_dis[u] + w;
                estimated_dis[v] = actual_dis[v] + Manhattan(v, end_id);
                prev[v] = u;
                pq.push({ estimated_dis[v],v });
            }
        }
    }
    ll s1 = end_id;
    ll s2 = 0;
    int s1x = -1, s1y = -1, s2x = -1, s2y = -1;
    while (s1 != start_id)//Reverse
    {
        s2 = prev[s1];
        if (s2 == 0)
        {
            cout << "Please choose again!!!" << endl;
            break;
        }
        s1x = nodes[s1].x;
        s1y = nodes[s1].y;
        s2x = nodes[s2].x;
        s2y = nodes[s2].y;
        if (!(((s1x >= ov_left && s1x <= ov_right) && (s1y >= ov_top && s1y <= ov_bottom)) || ((s2x >= ov_left && s2x <= ov_right) && (s2y >= ov_top && s2y <= ov_bottom))))//展示模式：显示遍历过的点
        {
            line(s2x, s2y, s1x, s1y);
            if (demonstrate) Sleep(10);//Demonstrate algorithm process
        }
        s1 = s2;
    }
    setlinestyle(PS_SOLID, linewidth);
}

void Welsh_Powell()//Welsh-Powell algorithm
{
    degree.resize(shelters.size());//
    visited.resize(shelters.size(), false);//
    colors.resize(shelters.size(), -1);//
    dual_adj.resize(shelters.size());//Reset vectors
    for (auto& p : dest)
    {
        ll id1 = p.first;
        int color1 = p.second.first;
        for (size_t i = 0; i < graph[id1].size(); i++)//Consider every node of the graph
        {
            ll id2 = graph[id1][i].to;
            int color2 = dest[id2].first;
            if (color1 != color2)
            {
                dual_adj[color1].insert(color2);
                dual_adj[color2].insert(color1);
            }
        }
    }

    for (int i = 0; i < degree.size(); ++i)
    {
        for (auto& p : dual_adj[i])
        {
            degree[i].push({ dual_adj[p].size(), p });
        }
    }
    int maxdegree = -1, maxdegree_node_id = -1;//Node with greatest degree
    for (int i = 0; i < dual_adj.size(); ++i)
    {
        if ((int)dual_adj[i].size() > maxdegree)
        {
            maxdegree = dual_adj[i].size();
            maxdegree_node_id = i;
        }
    }
    Greedy(maxdegree_node_id);
    for (int i = 0; i < colors.size(); ++i)
    {
        cout << i << ": " << colors[i] << endl;
    }
    cout << dual_adj[maxdegree_node_id].size() << endl;
}

void Greedy(int node_id)//Greedy algorithm
{
    visited[node_id] = true;
    int color_id = 0;
    while (true)
    {
        bool judge = true;
        for (auto& ne : dual_adj[node_id])
        {
            if (visited[ne] && colors[ne] == color_id) judge = false;
        }
        if (judge) break;
        color_id++;
    }
    colors[node_id] = color_id;
    while (!degree[node_id].empty())
    {
        auto t = degree[node_id].top();
        degree[node_id].pop();
        if (visited[t.second]) continue;
        Greedy(t.second);
    }
    return;
}