#include "PJ.h"

void Drawpart(const vector<Way>& ways_type, COLORREF ways_color, int waytype)//Draw different types of ways
{
    setlinecolor(ways_color);//Set color according to way type
    if (waytype == WAY_TYPE_HIGHWAY) setlinestyle(PS_SOLID, 1 + linewidth);//
    if (waytype == WAY_TYPE_NATURAL) setlinestyle(PS_SOLID, 3 + linewidth*2);//Bolder highways and rivers
    int x1, y1, x2, y2;
    for (const auto& way : ways_type)
    {
        if (way.type == WAY_TYPE_NATURAL && way.nodeRefs.size() < 20) continue;
        for (size_t i = 0; i < way.nodeRefs.size() - 1; i++)
        {
            x1 = mid_width + static_cast<int>((nodes[way.nodeRefs[i]].lon - 104.0824) * WINDOW_WIDTH / ratio);//
            y1 = mid_height + static_cast<int>((30.6582 - nodes[way.nodeRefs[i]].lat) * WINDOW_WIDTH / ratio);//
            nodes[way.nodeRefs[i]].x = x1;
            nodes[way.nodeRefs[i]].y = y1;
            x2 = mid_width + static_cast<int>((nodes[way.nodeRefs[i + 1]].lon - 104.0824) * WINDOW_WIDTH / ratio);//
            y2 = mid_height + static_cast<int>((30.6582 - nodes[way.nodeRefs[i + 1]].lat) * WINDOW_WIDTH / ratio);//calculate relative coordinates
            nodes[way.nodeRefs[i + 1]].x = x2;
            nodes[way.nodeRefs[i + 1]].y = y2;

            if (!((x1 >= 0 && x1 <= WINDOW_WIDTH && y1 >= 0 && y1 <= WINDOW_HEIGHT) || (x2 >= 0 && x2 <= WINDOW_WIDTH && y2 >= 0 && y2 <= WINDOW_HEIGHT)))
            {
                continue;//Skip invisible parts
            }
            else line(x1, y1, x2, y2);
        }
    }
    if (waytype == WAY_TYPE_HIGHWAY || waytype == WAY_TYPE_NATURAL) setlinestyle(PS_SOLID, linewidth);//Reset line style
}

void Drawmap()//Draw the map
{
    if (Mode == MODE_NORMAL) setbkcolor(COLOR_BKGROUND_NORMAL);//
    if (Mode == MODE_DOOMSDAY) setbkcolor(COLOR_BKGROUND_DOOMSDAY);//Background color 

    cleardevice();//Clear
    nodesinsight.clear();
    judge_shelter = false;

    if (ratio >= 0.08) linewidth = 1;//
    else if (ratio >= 0.04) linewidth = 2;//
    else if (ratio >= 0.02) linewidth = 3;//
    else linewidth = 4;//
    setlinestyle(PS_SOLID, linewidth);//Change linestyle

    string key = to_string(mid_width) + "_" + to_string(mid_height) + "_" + to_string(ratio) + "_" + to_string(Mode);//Map hash key
    if (maps.find(key) != maps.end())//Find the drawn map
    {
        putimage(0, 0, &maps[key]);
        Init_nodesinsight();
        int x1, y1, x2, y2;
        for (const auto& way : ways_highway)
        {
            for (size_t i = 0; i < way.nodeRefs.size() - 1; i++)
            {
                x1 = mid_width + static_cast<int>((nodes[way.nodeRefs[i]].lon - 104.0824) * WINDOW_WIDTH / ratio);//
                y1 = mid_height + static_cast<int>((30.6582 - nodes[way.nodeRefs[i]].lat) * WINDOW_WIDTH / ratio);//
                nodes[way.nodeRefs[i]].x = x1;
                nodes[way.nodeRefs[i]].y = y1;
                x2 = mid_width + static_cast<int>((nodes[way.nodeRefs[i + 1]].lon - 104.0824) * WINDOW_WIDTH / ratio);//
                y2 = mid_height + static_cast<int>((30.6582 - nodes[way.nodeRefs[i + 1]].lat) * WINDOW_WIDTH / ratio);//calculate relative coordinates
                nodes[way.nodeRefs[i + 1]].x = x2;
                nodes[way.nodeRefs[i + 1]].y = y2;
            }
        }
        return;
    }

    if(Mode==MODE_NORMAL) Drawpart(ways_highway, COLOR_HIGHWAY, WAY_TYPE_HIGHWAY);//
    else Drawpart(ways_highway, COLOR_HIGHWAY_DOOMSDAY, WAY_TYPE_HIGHWAY);//
    Drawpart(ways_natural, COLOR_NATURAL, WAY_TYPE_NATURAL);//
    if (ratio < 0.25) Drawpart(ways_barrier, COLOR_BARRIER, WAY_TYPE_BARRIER);//
    if (ratio < 0.125)//
    {
        Drawpart(ways_leisure, COLOR_LEISURE, WAY_TYPE_LEISURE);//
        Drawpart(ways_amenity, COLOR_AMENITY, WAY_TYPE_AMENITY);//
    }
    if (ratio < 0.0625) Drawpart(ways_building, COLOR_BUILDING, WAY_TYPE_BUILDING);//Draw parts

    Overview();//Draw overview
    Scale();//Draw scale
    Printmode();
    getimage(&maps[key], 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);//Store map
    Init_nodesinsight();
    if (Mode == MODE_DOOMSDAY)//Initialize shelters coordinate
    {
        shelters_nodes.clear();
        for (int i = 0; i < shelters.size(); ++i)
        {
            auto p = shelters[i];
            Node n;
            n.id = 0x7fffffff - p.lat * 10000;
            n.lat = p.lat;
            n.lon = p.lon;
            n.x = mid_width + static_cast<int>((p.lon - 104.0824) * WINDOW_WIDTH / ratio);
            n.y = mid_height + static_cast<int>((30.6582 - p.lat) * WINDOW_WIDTH / ratio);
            nodes[n.id] = n;
            shelters_nodes.push_back(n);
        }
    }
}

void Drawspot(int x, int y, string mode)//Draw the chosen Dijkstra & Astar's closest nodes
{
    if (mode == "Dijkstra") setfillcolor(RED);
    if (mode == "Astar") setfillcolor(BLUE);
    solidcircle(x, y, 5 + 0.25 / ratio);
}

void Drawdistribution()//Color the nodes of different shelter area
{
    int x_d, y_d;
    COLORREF c_d;
    for (auto& d : dest)
    {
        x_d = nodesinsight[d.first].x;
        y_d = nodesinsight[d.first].y;
        if (x_d >= ov_left && x_d <= ov_right && y_d >= ov_top && y_d <= ov_bottom)
        {
            continue;
        }
        if (x_d >= sc_left && x_d <= sc_right && y_d >= sc_top && y_d <= sc_bottom)
        {
            continue;
        }
        if (x_d >= mode_left && x_d <= mode_right && y_d >= mode_top && y_d <= mode_bottom)
        {
            continue;
        }
        c_d = distribute_color[2 * colors[d.second.first]];
        setfillcolor(c_d);
        solidcircle(x_d, y_d, 2);
    }
}