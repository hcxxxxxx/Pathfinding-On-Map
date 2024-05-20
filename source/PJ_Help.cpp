#include "PJ.h"

void Overview()//Draw overview
{
    int ov_mid_x = (ov_left + ov_right) / 2;//
    int ov_mid_y = (ov_top + ov_bottom) / 2;//Overview central coordinate
    int area_width = ov_width * 4 * ratio;//
    int area_height = ov_height * 4 * ratio;//Overview visible area central coordinate

    int left = area_mid_x - area_width / 2;//
    int top = area_mid_y - area_height / 2;//
    int right = min(left + area_width, ov_right);//
    int bottom = min(top + area_height, ov_bottom);//
    left = max(left, ov_left);//
    top = max(top, ov_top);//Overview visible area edge

    setfillcolor(COLOR_OVERVIEW_BACKGROUND);//
    solidrectangle(984, 40, 1240, 232);//Background(dark)
    setfillcolor(WHITE);//
    solidrectangle(left, top, right, bottom);//Background(light)
    setlinestyle(PS_SOLID, 1);
    for (const auto& way : ways_highway)//Draw only highway
    {
        if (way.stype != "trunk" && way.stype != "primary" && way.stype != "secondary" && way.stype != "tertiary") continue;

        int ox1 = -1, oy1 = -1, ox2 = -1, oy2 = -1;//Nodes on overview
        string os1, os2;
        for (size_t i = 0; i < way.nodeRefs.size() - 1; i++)
        {
            ox1 = ov_mid_x + static_cast<int>((nodes[way.nodeRefs[i]].lon - 104.0824) * 256 / 0.25);//
            oy1 = ov_mid_y + static_cast<int>((30.6582 - nodes[way.nodeRefs[i]].lat) * 256 / 0.25);//
            ox2 = ov_mid_x + static_cast<int>((nodes[way.nodeRefs[i + 1]].lon - 104.0824) * 256 / 0.25);//
            oy2 = ov_mid_y + static_cast<int>((30.6582 - nodes[way.nodeRefs[i + 1]].lat) * 256 / 0.25);//Calculate coordinates on overview

            if (ox1 <= ov_left || ox1 >= ov_right || oy1 <= ov_top || oy1 >= ov_bottom || ox2 <= ov_left || ox2 >= ov_right || oy2 <= ov_top || oy2 >= ov_bottom) continue;//
            if (ox1 >= left && ox1 <= right && oy1 >= top && oy1 <= bottom || ox2 >= left && ox2 <= right && oy2 >= top && oy2 <= bottom) setlinecolor(GREEN);//Visible line color
            else setlinecolor(RGB(105, 139, 105));//Invisible line color
            line(ox1, oy1, ox2, oy2);//Draw the line
        }
    }
    setlinestyle(PS_SOLID, 2);//
    setlinecolor(BLACK);//
    rectangle(ov_left, ov_top, ov_right, ov_bottom);//Black frame
    setlinecolor(GREEN);//
    rectangle(left, top, right, bottom);//Green frame of visible area
    setlinestyle(PS_SOLID, linewidth);//Reset linestyle
}

void Scale()//Draw scale
{
    settextstyle(16, 0, _T("Centaur"), 0, 0, FW_DEMIBOLD, 0, 0, 0);
    setfillcolor(RGB(224, 255, 255));
    solidrectangle(sc_left, sc_top, sc_right, sc_bottom);
    settextcolor(RED);
    setlinestyle(PS_SOLID, 2);
    setlinecolor(BLACK);
    int lx = sc_left + (sc_bottom - sc_top) / 2;
    int rx = lx + 92;
    int y = (sc_top + sc_bottom) / 2;
    line(lx, y - 5, lx, y + 5);
    line(rx, y - 5, rx, y + 5);
    line(lx, y, rx, y);
    rectangle(sc_left, sc_top, sc_right, sc_bottom);
    setbkmode(TRANSPARENT);
    int r = ratio / 0.00390625;
    switch (r)
    {
    case 512:
    {
        TCHAR s512[] = _T("16 km");
        outtextxy(rx + 20, y - 8, s512);
        break;
    }
    case 256:
    {
        TCHAR s256[] = _T("8 km");
        outtextxy(rx + 20, y - 8, s256);
        break;
    }
    case 128:
    {
        TCHAR s128[] = _T("4 km");
        outtextxy(rx + 20, y - 8, s128);
        break;
    }
    case 64:
    {
        TCHAR s64[] = _T("2 km");
        outtextxy(rx + 20, y - 8, s64);
        break;
    }
    case 32:
    {
        TCHAR s32[] = _T("1 km");
        outtextxy(rx + 20, y - 8, s32);
        break;
    }
    case 16:
    {
        TCHAR s16[] = _T("500 m");
        outtextxy(rx + 20, y - 8, s16);
        break;
    }
    case 8:
    {
        TCHAR s8[] = _T("250 m");
        outtextxy(rx + 20, y - 8, s8);
        break;
    }
    case 4:
    {
        TCHAR s4[] = _T("125 m");
        outtextxy(rx + 20, y - 8, s4);
        break;
    }
    case 2:
    {
        TCHAR s2[] = _T("62.5 m");
        outtextxy(rx + 20, y - 8, s2);
        break;
    }
    case 1:
    {
        TCHAR s1[] = _T("31.25 m");
        outtextxy(rx + 20, y - 8, s1);
        break;
    }
    }
}

void Printmode()//Print mode information
{
    setfillcolor(RGB(224, 255, 255));
    solidrectangle(mode_left, mode_top, mode_right, mode_bottom);
    setlinestyle(PS_SOLID, 2);
    setlinecolor(BLACK);
    rectangle(mode_left, mode_top, mode_right, mode_bottom);

    settextstyle(20, 0, _T("Centaur"), 0, 0, FW_DEMIBOLD, 0, 0, 0);
    if (Mode == MODE_NORMAL) outtextxy(mode_left + 20, mode_top + 5, _T("Normal Mode"));
    else outtextxy(mode_left + 20, mode_top + 5, _T("Doomsday Mode"));
}

void Help()//Print help information
{
    cout << "      ######  ###   ##  ######  ####    ######     ###   ###     ###   #######  ######  ####    ###   ##           " << endl;
    cout << "       ##    ####  ##  ##     ##   ##  ##    ##   ####  ####    ####     ##      ##   ##   ##  ####  ##            " << endl;
    cout << "      ##    ## ## ##  #####  ##    ## #######    ## ## ## ##   ## ##    ##      ##   ##    ## ## ## ##             " << endl;
    cout << "     ##    ##  ####  ##      ##   ## ##   ##    ##  ####  ##  ######   ##      ##    ##   ## ##  ####              " << endl;
    cout << "  ######  ##   ###  ##        ####  ##     ##  ##   ###   ## ##   ##  ##    ######    ####  ##   ###               " << endl;
    cout << endl;
    cout << "KEYBOARD FUNCTIONS:                                                            **                                  " << endl;
    cout << "                                                **    **    *******  **   **  **  *****                            " << endl;
    cout << "        ESC   :    Reset map view               **    **   **         ** **      **   **                           " << endl;
    cout << "        Q     :    View bigger                  ********  **           ***        ***                              " << endl;
    cout << "        E     :    View smaller                 **    **  **           ***          ***                            " << endl;
    cout << "        W/¡ü   :    View up                      **    **   **         ** **      **   **                           " << endl;
    cout << "        S/¡ý   :    View down                    **    **    *******  **   **      *****                            " << endl;
    cout << "        A/¡û   :    View left                                                                                       " << endl;
    cout << "        D/¡ú   :    View right                                       **       **       *       *******              " << endl;
    cout << "        SPACE :    Clear                                            ***     ***      ***      **     **            " << endl;
    cout << "        CTRL  :    Normal mode  :   Change mode of demonstration    ****   ****     ** **     **     **            " << endl;
    cout << "        TAB   :    Normal mode  :   Change mode: Doomsday mode      ** ** ** **    **   **    *******              " << endl;
    cout << "                   Doomsday mode:   Change mode: Normal mode        **  ***  **   *********   **                   " << endl;
    cout << "        P     :    Doomsday mode:   Show shelter locations          **   *   **  **       **  **                   " << endl;
    cout << "        O     :    Doomsday mode:   Show recommended shelter area(Please show shelter locations first)             " << endl;
    cout << "        H     :    Print help information" << endl;
    cout << endl;
    cout << "MOUSE FUNCTIONS: " << endl;
    cout << endl;
    cout << "        LEFT CLICK:    Normal mode  :   Choose Dijkstra starting & ending node" << endl;
    cout << "                       Doomsday mode£º  Add new shelters" << endl;
    cout << "        RIGHT CLICK:   Normal mode  :   Choose Astar starting & ending node" << endl;
    cout << "        WHEEL UP:      View bigger" << endl;
    cout << "        WHEEL DOWN:    View smaller" << endl;
}