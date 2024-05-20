#include "PJ.h"

void Init_shelters()//Initialize shelters' closest nodes
{
    shelters_closest.clear();
    for (int i = 0; i < shelters_nodes.size(); ++i)
    {
        int mindis = 10000000;
        int dis;
        int x = shelters_nodes[i].x;
        int y = shelters_nodes[i].y;
        ll id = -1;
        for (auto& pair : nodesinsight)
        {
            dis = abs(x - pair.second.x) + abs(y - pair.second.y);
            if (dis < mindis)
            {
                mindis = dis;
                id = pair.second.id;
            }
        }
        shelters_closest.push_back(nodesinsight[id]);
    }
}

void Shelter()//Draw shelters
{
    settextstyle(16, 0, _T("Centaur"), 0, 0, FW_ULTRABOLD, 0, 0, 0);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 2);
    for (int i = 0; i < shelters_closest.size(); ++i)
    {
        auto p = shelters[i];
        int x = mid_width + static_cast<int>((p.lon - 104.0824) * WINDOW_WIDTH / ratio);
        int y = mid_height + static_cast<int>((30.6582 - p.lat) * WINDOW_WIDTH / ratio);
        wstring wtext(p.name.begin(), p.name.end());
        LPCTSTR lptstr = wtext.c_str();//The name of the newly built shelter
        settextcolor(BLACK);
        outtextxy(x - 40, y + 10, lptstr);
        outtextxy(x - 38, y + 8, lptstr);
        settextcolor(WHITE);
        outtextxy(x - 39, y + 9, lptstr);
    }
    for (int i = 0; i < shelters_closest.size(); ++i)
    {
        auto p = shelters[i];
        setfillcolor(distribute_color[2 * colors[i]]);
		int x = mid_width + static_cast<int>((p.lon - 104.0824) * WINDOW_WIDTH / ratio);//
        int y = mid_height + static_cast<int>((30.6582 - p.lat) * WINDOW_WIDTH / ratio);//Calculate coordinate
		fillcircle(x, y, 5 + 0.25 / ratio);
        int dx1 = 0, dy1 = 0, dx2 = -10, dy2 = -20, dx3 = 0, dy3 = -40, dx4 = 10, dy4 = -20;
        int dx2_2 = 5;
        POINT pts[] = { {x + dx1,y + dy1},{x + dx2,y + dy2},{x + dx3,y + dy3},{x + dx4,y + dy4} };
        POINT pts2[] = { {x + dx1,y + dy1},{x + dx2_2,y + dy2}, {x + dx3,y + dy3},{x + dx4,y + dy4} };
        fillpolygon(pts, 4);
        setfillcolor(distribute_color[2 * colors[i] + 1]);
        fillpolygon(pts2, 4);
    }
}

void Distribute_shelters()//Distribute shelters using greedy algorithm
{
    Init_shelters();
    const int INF = 0x7fffffff;
    for (auto& n : nodesinsight)
    {
        dest[n.first] = { -1,INF };
    }
    for (int i = 0; i < shelters_closest.size(); ++i)
    {
        ll id = shelters_closest[i].id;//Shelter id
        for (auto& p : nodesinsight)
        {
            ll pid = p.first;
            int dist = Euclid(id, pid);
            if (dist < dest[pid].second || dest[pid].first == -1)
            {
                dest[pid] = { i,dist };
            }
        }
    }
    Welsh_Powell();
}

void Insert_shelters(int x, int y)//Add new shelters
{
    wchar_t s[100];
    InputBox(s, 100, L"Name of the new shelter", L"Insert a shelter", L"New Shelter");

    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
    char* buffer = new char[bufferSize];
    WideCharToMultiByte(CP_UTF8, 0, s, -1, buffer, bufferSize, NULL, NULL);
    string str(buffer);
    delete[] buffer;

    Node n;
    int mindis = 10000000;
    int dis;
    for (auto& pair : nodesinsight)//Finding closest node using Manhattan distance
    {
        dis = abs(x - pair.second.x) + abs(y - pair.second.y);
        if (dis < mindis)
        {
            mindis = dis;
            n = pair.second;
        }
    }
    int _x = n.x;
    int _y = n.y;
    shelters.push_back(Shelter_t(n.lat, n.lon, str));
    shelters_nodes.push_back(n);
}