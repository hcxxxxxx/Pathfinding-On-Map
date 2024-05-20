#include "PJ.h"

void Multifunc()//Multi-function
{
    bool demonstrate = false;//Judge if demonstrate the process of algorithm(Default:not demonstrating)
    int countspot = 0;//Node counter in Dijkstra & Astar
    ExMessage ms;
    while (1)
    {
        ms = getmessage(EX_MOUSE | EX_KEY);
        switch (ms.message)//Keyboard & Mouse information
        {
        case WM_KEYDOWN:
            countspot = 0;//Reset numbers of chosen nodes
            if (ms.vkcode == VK_ESCAPE)//'Esc',reset maps
            {
                ratio = 0.25;//Reset ratio
                mid_height = WINDOW_HEIGHT / 2;//
                mid_width = WINDOW_WIDTH / 2;//Reset central coordinate
                area_mid_x = (ov_left + ov_right) / 2;//
                area_mid_y = (ov_top + ov_bottom) / 2;//Reset overview visible area central coordinate
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == 0x51)//'Q',bigger
            {
                if (ratio > 0.005) ratio /= 2.0;//Change ratio(Minimized 0.005)
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == 0x45)//'E',smaller
            {
                ratio *= 2.0;//Change ratio
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == 0x57 || ms.vkcode == VK_UP)//'W' or '¡ü',up
            {
                mid_height += WINDOW_HEIGHT / 8;//Change central coordinate
                area_mid_y -= ov_height * ratio / 2;//Change overview visible area central coordinate
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == 0x53 || ms.vkcode == VK_DOWN)//'S' or '¡ý',down
            {
                mid_height -= WINDOW_HEIGHT / 8;//Change central coordinate
                area_mid_y += ov_height * ratio / 2;//Change overview visible area central coordinate
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == 0x41 || ms.vkcode == VK_LEFT)//'A' or '¡û',left
            {
                mid_width += WINDOW_WIDTH / 8;//Change central coordinate
                area_mid_x -= ov_width * ratio / 2;//Change overview visible area central coordinate
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == 0x44 || ms.vkcode == VK_RIGHT)//'D' or '¡ú',right
            {
                mid_width -= WINDOW_WIDTH / 8;//Change central coordinate
                area_mid_x += ov_width * ratio / 2;//Change overview visible area central coordinate
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == VK_SPACE)//' ',Clear
            {
                countspot = 0;
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == VK_CONTROL)//'Ctrl',change demonstrate mode
            {
                countspot = 0;
                demonstrate = !demonstrate;//Change demonstrate mode
                Drawmap();//Re-draw
            }
            else if (ms.vkcode == 0x48)//'H',print help
            {
                Help();//Print
            }
            else if (ms.vkcode == 0x50 && Mode == MODE_DOOMSDAY)//'P',draw shelters
            {
                Distribute_shelters();
                Shelter();
                judge_shelter = true;
            }
            else if (ms.vkcode == 0x4f && Mode == MODE_DOOMSDAY)//'O',color the nodes
            {
                if (judge_shelter)
                {
                    Drawdistribution();
                    Shelter();
                }
            }
            else if (ms.vkcode == VK_TAB)//'Tab',change the mode
            {
                if (Mode == MODE_NORMAL) Mode = MODE_DOOMSDAY;
                else if (Mode == MODE_DOOMSDAY) Mode = MODE_NORMAL;
                Init_graph();
                Drawmap();
            }
            break;
        case WM_LBUTTONDOWN:
            if (Mode == MODE_NORMAL)//Dijkstra nodes choosing
            {
                if (countspot == 0)//Dijkstra starting node
                {
                    countspot++;
                    m_x1 = ms.x;
                    m_y1 = ms.y;
                    Closest(1);//Find closest node
                    Drawspot(n_1.x, n_1.y, "Dijkstra");//Draw closest node
                }
                else//Dijkstra ending node
                {
                    countspot = 0;
                    m_x2 = ms.x;
                    m_y2 = ms.y;
                    Closest(2);//Find closest node
                    Drawspot(n_2.x, n_2.y, "Dijkstra");//Draw closest node
                    Dijkstra(demonstrate);//Dijkstra
                }
            }
            else if (Mode == MODE_DOOMSDAY)//Add shelter
            {
                shelters_closest.clear();
                degree.clear();
                visited.clear();
                colors.clear();
                dual_adj.clear();
                Insert_shelters(ms.x, ms.y);
                Distribute_shelters();
                Shelter();
                judge_shelter = true;
            }
            break;
        case WM_RBUTTONDOWN://Astar nodes choosing
            if (countspot == 0)//Astar starting node
            {
                countspot++;
                m_x1 = ms.x;
                m_y1 = ms.y;
                Closest(1);//Find closest node
                Drawspot(n_1.x, n_1.y, "Astar");//Draw closest node
            }
            else//Astar ending node
            {
                countspot = 0;
                m_x2 = ms.x;
                m_y2 = ms.y;
                Closest(2);//Find closest node
                Drawspot(n_2.x, n_2.y, "Astar");//Draw closest node
                cout << "Graph size before Astar: " << graph.size() << endl;
                Astar(demonstrate);//Astar
            }
            break;
        case WM_MOUSEWHEEL:
            countspot = 0;
            if (ms.wheel > 0 && ratio > 0.005) ratio /= 2.0;//Wheel up,bigger
            else ratio *= 2.0;//Wheel down,smaller
            Drawmap();//Re-draw
            break;
        }
    }
}