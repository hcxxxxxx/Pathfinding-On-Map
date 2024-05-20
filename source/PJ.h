#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include "graphics.h"
#include "pugixml.hpp"
#include <set>
#include <conio.h>
#include <locale>
#include <codecvt>
#include <time.h>

#define MAXsSTAR 500//Small star numbers
#define MAXbSTAR 150//Big star numbers
#define WINDOW_WIDTH  1280//
#define WINDOW_HEIGHT 960//Window's size   1280*960

#define WAY_TYPE_BUILDING 1//
#define WAY_TYPE_HIGHWAY 2//
#define WAY_TYPE_NATURAL 3//
#define WAY_TYPE_BARRIER 4//
#define WAY_TYPE_LEISURE 5//
#define WAY_TYPE_AMENITY 6//Ways type

#define MODE_NORMAL 1//
#define MODE_DOOMSDAY 2//

#define COLOR_BKGROUND_NORMAL RGB(238, 233, 233)//
#define COLOR_BKGROUND_DOOMSDAY RGB(65, 16, 20)//
#define COLOR_BUILDING RGB(139, 137, 137)//
#define COLOR_HIGHWAY RGB(139, 121, 94)//
#define COLOR_HIGHWAY_DOOMSDAY RGB(105, 105, 105)//
#define COLOR_NATURAL RGB(72, 118, 255)//
#define COLOR_BARRIER RGB(238, 220, 130)//
#define COLOR_LEISURE RGB(255, 193, 193)//
#define COLOR_AMENITY RGB(139, 101, 8)//
#define COLOR_DIJ_TRAVERSED_NODES RGB(255, 165, 0)//
#define COLOR_ASTAR_TRAVERSED_NODES RGB(148, 0, 211)//
#define COLOR_ASTAR_ROAD RGB(0, 191, 255)//
#define COLOR_OVERVIEW_BACKGROUND RGB(190, 190, 190)//
#define COLOR_NAVIGATE_1 RGB(255, 255, 0)//
#define COLOR_NAVIGATE_2 RGB(255, 215, 0)//
#define COLOR_NAVIGATE_3 RGB(238, 180, 34)//Colors type

#define ov_left 984//
#define ov_top 40//
#define ov_right 1240//
#define ov_bottom 232//Overview position & size   256*192

#define sc_left 50//
#define sc_top 860//
#define sc_right 230//
#define sc_bottom 890//Scale position & size   180*30

#define mode_left 50//
#define mode_top 820//
#define mode_right 230//
#define mode_bottom 850//Mode position & size   180*30

// minlon=104.0036;   maxlon=104.1612;
// minlat=30.5976;    maxlat=30.7187;
//
// map's width  = 0.1576
// map's height = 0.1211
// 
// center: lon = 104.0824 , lat = 30.6582

using namespace std;
using ll = long long;

struct sStar
{
    double x[4]; int y[4]; double step; int color;
};//sStar struct

struct bStar
{
    double x[12]; int y[12]; double step; int color;
};//bStar struct

struct Node
{
    string name; ll id; double lat; double lon; int x; int y;
};//Node struct

struct Way
{
    string name; vector<ll> nodeRefs; int type; string stype;
};//Way struct

struct Edge
{
    ll to; int weight;
    Edge(ll _to, int _weight) :to(_to), weight(_weight) {}
};//Edge struct in Dijkstra & Astar

struct Shelter_t
{
    double lat; double lon; string name;
    Shelter_t(double _lat, double _lon, string _name) :lat(_lat), lon(_lon), name(_name) {}
};


extern bool init;//Judge if uninitialized
extern bool judge_shelter;//Judge if shelters are drawn

extern int Mode;//Map mode(Default:Normal)
extern int mid_width;//
extern int mid_height;//Window's central coordinate
extern int linewidth;//Linewidth(Default:1)
extern int m_x1, m_y1, m_x2, m_y2;//Click point 1 & 2 in Dijkstra & Astar
extern int area_mid_x, area_mid_y;//Central coordinate of overview visible area
extern int ov_width, ov_height;//Overview width & height

extern double ratio;//Ratio(Default:0.25)

extern Node n_1, n_2;//Nearest point 1 & 2 in Dijkstra & Astar
extern sStar sstar[MAXsSTAR];//sStars
extern bStar bstar[MAXbSTAR];//bStars

extern vector<bool> visited;//Judge if visited 
extern vector<int> colors;//Colors
extern vector<set<int>> dual_adj;//Adjacency list of dual graph
extern vector<Node> shelters_closest;//
extern vector<Node> shelters_nodes;//Shelters hash
extern vector<Way> ways;//Ways
extern vector<Way> ways_highway, ways_building, ways_natural, ways_barrier, ways_leisure, ways_amenity;//Different types of ways
extern vector<Shelter_t> shelters;//Shelters' location
extern vector<priority_queue<pair<int, int>, vector<pair<int, int>>, less<pair<int, int>>>> degree;//Degrees of adjacent nodes(Big-topped)

extern unordered_map<ll, Node> nodes;//Nodes hash
extern unordered_map<ll, Node> nodesinsight;//Visible (highway) nodes hash
extern unordered_map<ll, vector<Edge>> graph;//Graph
extern unordered_map<ll, pair<int, int>> dest;//dest[id].first: The color-number of the node(the area as well)
extern unordered_map<string, IMAGE> maps;//Maps hash
extern COLORREF distribute_color[10];//Color lists



//Load
void Init_window();//Initialize the window
void Loadmap();//Load datas
void Init_nodesinsight();//Initialize visible nodes
void Init_graph();//Initialize graph
void Stars();//
void Initsstar(int i);//
void Initbstar(int i);//
void Movesstar(int i);//
void Movebstar(int i);//Initialize stars

//Multifunc
void Multifunc();//Multi-function

//Drawing
void Drawpart(const vector<Way>& ways_type, COLORREF ways_color, int waytype);//Draw different types of ways
void Drawmap();//Draw the map
void Drawspot(int x, int y, string mode);//Draw the chosen Dijkstra & Astar's closest nodes
void Drawdistribution();//Color the nodes of different shelter area

//Algorithm
int Euclid(ll start_key, ll end_key);//Calculate Euclid distance
int Manhattan(ll start_key, ll end_key);//Calculate Manhattan distance
void Closest(int n);//Find closest node according to the click point
void Dijkstra(bool demonstrate);//Dijkstra algorithm
void Astar(bool demonstrate);//Astar algorithm
void Welsh_Powell();//Welsh-Powell algorithm
void Greedy(int node_id);//Greedy algorithm

//Doomsday
void Init_shelters();//Initialize shelters' closest nodes
void Shelter();//Draw shelters
void Distribute_shelters();//Distribute shelters using greedy algorithm
void Insert_shelters(int x, int y);//Add new shelters

//Help
void Overview();//Draw overview
void Scale();//Draw scale
void Printmode();//Print mode information
void Help();//Print help information