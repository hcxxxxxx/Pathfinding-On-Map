#include "PJ.h"

void Init_window()//Initialize the window
{
    if (init)
    {
        initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
        HWND consoleWindow = GetConsoleWindow();
        ShowWindow(consoleWindow, SW_MINIMIZE);//Minimize console window
        init = false;
    }
    Stars();
}

void Loadmap()//Load datas
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../mapofChengdu.xml");//Load file

    if (!result)//If fail to load
    {
        cout << "Failed to load XML file." << endl;//Print
        return;
    }

    for (pugi::xml_node node : doc.child("osm").children("node"))//Load nodes
    {
        Node n;
        n.id = node.attribute("id").as_llong();//
        n.lat = node.attribute("lat").as_double();//
        n.lon = node.attribute("lon").as_double();//
        n.x = mid_width + static_cast<int>((n.lon - 104.0824) * WINDOW_WIDTH / ratio);//
        n.y = mid_height + static_cast<int>((30.6582 - n.lat) * WINDOW_WIDTH / ratio);//
        nodes[n.id] = n;//Nodes hash
    }

    for (pugi::xml_node way : doc.child("osm").children("way"))//Load ways
    {
        Way w;
        for (pugi::xml_node nd : way.children("nd"))
        {
            long long ref = nd.attribute("ref").as_llong();
            w.nodeRefs.push_back(ref);//
        }
        for (pugi::xml_node tag : way.children("tag"))//Load ways type
        {
            string key = tag.attribute("k").value();
            if (key == "highway")
            {
                w.type = WAY_TYPE_HIGHWAY;//
                w.stype = tag.attribute("v").value();
            }
            if (key == "building") w.type = WAY_TYPE_BUILDING;//
            if (key == "natural") w.type = WAY_TYPE_NATURAL;//
            if (key == "barrier") w.type = WAY_TYPE_BARRIER;//
            if (key == "leisure") w.type = WAY_TYPE_LEISURE;//
            if (key == "amenity") w.type = WAY_TYPE_AMENITY;//
        }

        switch (w.type)//Load different types of ways
        {
        case WAY_TYPE_HIGHWAY:
            ways_highway.push_back(w);
            break;
        case WAY_TYPE_BUILDING:
            ways_building.push_back(w);
            break;
        case WAY_TYPE_NATURAL:
            ways_natural.push_back(w);
            break;
        case WAY_TYPE_BARRIER:
            ways_barrier.push_back(w);
            break;
        case WAY_TYPE_LEISURE:
            ways_leisure.push_back(w);
            break;
        case WAY_TYPE_AMENITY:
            ways_amenity.push_back(w);
            break;
        }

        ways.push_back(w);
    }
}

void Init_nodesinsight()//Initialize visible nodes
{
    nodesinsight.clear();
    for (const auto& way : ways_highway)//Visible highway nodes
    {
        for (size_t i = 0; i < way.nodeRefs.size(); i++)
        {
            ll id1 = way.nodeRefs[i];
            Node n = nodes[id1];
            n.x = mid_width + static_cast<int>((nodes[way.nodeRefs[i]].lon - 104.0824) * WINDOW_WIDTH / ratio);
            n.y = mid_height + static_cast<int>((30.6582 - nodes[way.nodeRefs[i]].lat) * WINDOW_WIDTH / ratio);
            if (!(n.x >= 0 && n.x <= WINDOW_WIDTH && n.y >= 0 && n.y <= WINDOW_HEIGHT))
            {
                continue;
            }
            nodesinsight[id1] = n;
        }
    }
}

void Init_graph()//Initialize graph
{
    ll id1 = -1, id2 = -1;
    for (const auto& way : ways_highway)//Initialize adjacency list
    {
        for (size_t i = 0; i < way.nodeRefs.size() - 1; i++)
        {
            id1 = way.nodeRefs[i];
            id2 = way.nodeRefs[i + 1];
            graph[id1].push_back(Edge(id2, Manhattan(id1, id2)));
            graph[id2].push_back(Edge(id1, Manhattan(id1, id2)));
        }
    }
}

void Stars()
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < MAXsSTAR; ++i)
    {
        Initsstar(i);
        sstar[i].x[0] = rand() % WINDOW_WIDTH;
        sstar[i].x[1] = sstar[i].x[0] + 1;
        sstar[i].x[2] = sstar[i].x[0];
        sstar[i].x[3] = sstar[i].x[1];
    }
    for (int i = 0; i < MAXbSTAR; ++i)
    {
        Initbstar(i);
        bstar[i].x[0] = rand() % WINDOW_WIDTH;
        bstar[i].x[1] = bstar[i].x[0] + 1;
        bstar[i].x[2] = bstar[i].x[0] - 1;
        bstar[i].x[3] = bstar[i].x[0];
        bstar[i].x[4] = bstar[i].x[0] + 1;
        bstar[i].x[5] = bstar[i].x[0] + 2;
        bstar[i].x[6] = bstar[i].x[0] - 1;
        bstar[i].x[7] = bstar[i].x[0];
        bstar[i].x[8] = bstar[i].x[0] + 1;
        bstar[i].x[9] = bstar[i].x[0] + 2;
        bstar[i].x[10] = bstar[i].x[0];
        bstar[i].x[11] = bstar[i].x[0] + 1;
    }

    ExMessage* ms = NULL;//
    TCHAR text[40] = _T("Hello! Welcome to hcx's city!!");//
    int incr = 255 / 160;//
    int cnt = 0;//
    LOGFONT f;//
    gettextstyle(&f);//
    f.lfHeight = 80;//
    _tcscpy_s(f.lfFaceName, _T("Comic Sans MS"));//
    f.lfQuality = ANTIALIASED_QUALITY;//
    f.lfWeight = FW_ULTRABOLD;//
    settextstyle(&f);//
    int text_width = WINDOW_WIDTH * 0.618;//Loading scene

    while (1)
    {
        if (peekmessage(ms, EX_KEY, true)) break;
        if (cnt < 160)
        {
            cnt++;
            settextcolor(RGB(incr * cnt, incr * cnt, incr * cnt));
        }
        outtextxy((WINDOW_WIDTH - text_width) / 2, WINDOW_HEIGHT / 2, text);//
        for (int i = 0; i < MAXsSTAR; ++i) Movesstar(i);//
        for (int i = 0; i < MAXbSTAR; ++i) Movebstar(i);//Text & Stars
        Sleep(20);
    }
    cleardevice();
    int r_incr = GetRValue(COLOR_BKGROUND_NORMAL) / 80;
    int g_incr = GetGValue(COLOR_BKGROUND_NORMAL) / 80;
    int b_incr = GetBValue(COLOR_BKGROUND_NORMAL) / 80;
    settextcolor(COLOR_BKGROUND_NORMAL);
    outtextxy((WINDOW_WIDTH - text_width) / 2, WINDOW_HEIGHT / 2, text);
    int r = 0, g = 0, b = 0;
    //
    //
    //
    BeginBatchDraw();
    while (1)
    {
        r += r_incr;
        g += g_incr;
        b += b_incr;
        if (r >= GetRValue(COLOR_BKGROUND_NORMAL) || g >= GetGValue(COLOR_BKGROUND_NORMAL)) break;
        setbkcolor(RGB(r, g, b));
        cleardevice();
        outtextxy((WINDOW_WIDTH - text_width) / 2, WINDOW_HEIGHT / 2, text);
        FlushBatchDraw();
        Sleep(8);
    }
    EndBatchDraw();
    //
    //
    //
}

void Initsstar(int i)//Small star
{
    for (int j = 0; j < 4; ++j) sstar[i].x[j] = 0;
    sstar[i].y[0] = rand() % WINDOW_HEIGHT;
    sstar[i].y[1] = sstar[i].y[0];
    sstar[i].y[2] = sstar[i].y[0] + 1;
    sstar[i].y[3] = sstar[i].y[2];

    sstar[i].step = (rand() % 5000) / 1000.0 + 1;
    sstar[i].color = (int)(sstar[i].step * 255 / 6.0 + 0.5);
    sstar[i].color = RGB(sstar[i].color, sstar[i].color, sstar[i].color);
}

void Initbstar(int i)//Big star
{
    for (int j = 0; j < 16; ++j) bstar[i].x[j] = 0;
    bstar[i].y[0] = rand() % WINDOW_HEIGHT;
    bstar[i].y[1] = bstar[i].y[0];
    bstar[i].y[2] = bstar[i].y[0]+1;
    bstar[i].y[3] = bstar[i].y[0]+1;
    bstar[i].y[4] = bstar[i].y[0]+1;
    bstar[i].y[5] = bstar[i].y[0]+1;
    bstar[i].y[6] = bstar[i].y[0]+2;
    bstar[i].y[7] = bstar[i].y[0]+2;
    bstar[i].y[8] = bstar[i].y[0]+2;
    bstar[i].y[9] = bstar[i].y[0]+2;
    bstar[i].y[10] = bstar[i].y[0]+3;
    bstar[i].y[11] = bstar[i].y[0]+3;

    bstar[i].step = (rand() % 5000) / 1000.0 + 1;
    bstar[i].color = (int)(bstar[i].step * 255 / 6.0 + 0.5);
    bstar[i].color = RGB(bstar[i].color, bstar[i].color, bstar[i].color);
}

void Movesstar(int i)
{
    for (int j = 0; j < 4; ++j) putpixel((int)sstar[i].x[j], sstar[i].y[j], 0);
    for (int j = 0; j < 4; ++j) sstar[i].x[j] += sstar[i].step;
    if (sstar[i].x[0] > WINDOW_WIDTH) Initsstar(i);
    for (int j = 0; j < 4; ++j) putpixel((int)sstar[i].x[j], sstar[i].y[j], sstar[i].color);
}

void Movebstar(int i)
{
    for (int j = 0; j < 12; ++j) putpixel((int)bstar[i].x[j], bstar[i].y[j], 0);
    for (int j = 0; j < 12; ++j) bstar[i].x[j] += bstar[i].step;
    if (bstar[i].x[0] > WINDOW_WIDTH) Initbstar(i);
    for (int j = 0; j < 12; ++j) putpixel((int)bstar[i].x[j], bstar[i].y[j], bstar[i].color);
}