#include "PJ.h"

bool init = true;//Judge if uninitialized
bool judge_shelter = false;//Judge if shelters are drawn

int Mode = MODE_NORMAL;//Map mode(Default:Normal)
int mid_width = WINDOW_WIDTH / 2;//
int mid_height = WINDOW_HEIGHT / 2;//Window's central coordinate
int linewidth = 1;//Linewidth(Default:1)
int m_x1 = 0, m_y1 = 0, m_x2 = 0, m_y2 = 0;//Click point 1 & 2 in Dijkstra & Astar
int area_mid_x = (ov_left + ov_right) / 2, area_mid_y = (ov_top + ov_bottom) / 2;//Central coordinate of overview visible area
int ov_width = ov_right - ov_left, ov_height = ov_bottom - ov_top;//Overview width & height

double ratio = 0.25;//Ratio(Default:0.25)

Node n_1, n_2;//Nearest point 1 & 2 in Dijkstra & Astar
sStar sstar[MAXsSTAR];//sStars
bStar bstar[MAXbSTAR];//bStars


vector<bool> visited;//Judge if visited 
vector<int> colors;//Colors
vector<set<int>> dual_adj;//Adjacency list of dual graph
vector<Node> shelters_closest;//
vector<Node> shelters_nodes;////Shelters hash
vector<Way> ways;//Ways
vector<Way> ways_highway, ways_building, ways_natural, ways_barrier, ways_leisure, ways_amenity;//Different types of ways
vector<priority_queue<pair<int, int>, vector<pair<int, int>>, less<pair<int, int>>>> degree;//Degrees of adjacent nodes(Big-topped)


unordered_map<ll, Node> nodes;//Nodes hash
unordered_map<ll, Node> nodesinsight;//Visible (highway) nodes hash
unordered_map<ll, vector<Edge>> graph;//Graph
unordered_map<ll, pair<int, int>> dest;//dest[id].first: The color-number of the node(the area as well)
unordered_map<string, IMAGE> maps;//Maps hash
COLORREF distribute_color[10] = {	RGB(255, 255, 0),	RGB(255, 215, 0), RGB(0, 0, 255),	RGB(0, 0, 139),
									RGB(255, 0, 0),		RGB(139, 35, 35), RGB(0, 238, 0),	RGB(0, 139, 0),
									RGB(255, 0, 255),	RGB(148, 0, 211) };

vector<Shelter_t> shelters = { Shelter_t(30.6072,104.0481,"S. ShenXianShu Rd."),		Shelter_t(30.6083,104.0440,"N. XinLe St."),			Shelter_t(30.6956,104.0236,"YingMenKou Rd."),
							   Shelter_t(30.7033,104.0438,"TangChen Rd."),				Shelter_t(30.7036,104.0699,"E. YangHua Rd."),		Shelter_t(30.6962,104.0541,"QiuShi Rd."),
						       Shelter_t(30.6930,104.0960,"SanYou Rd."),				Shelter_t(30.6745,104.0881,"ZhaoZhongCi St."),		Shelter_t(30.6695,104.0984,"MengZhuiWan St."),
						       Shelter_t(30.6627,104.0431,"W. 2nd Sec.,1st Ring Rd."),	Shelter_t(30.6630,104.0569,"ShaoCheng Rd."),		Shelter_t(30.6600,104.1022,"ShuangLin Rd."),
						       Shelter_t(30.6532,104.0651,"2nd Sec.,S. RenMin Rd."),	Shelter_t(30.6497,104.0878,"ShuangHuaiShu St."),	Shelter_t(30.6157,104.0758,"W. ChangShouYuan St."),
						       Shelter_t(30.6891,104.0056,"JinZe Rd."),					Shelter_t(30.6773,104.0159,"Mid. QingJiang Rd."),	Shelter_t(30.6743,104.0266,"S. FuQinShiRen Rd."),
						       Shelter_t(30.6039,104.0739,"TianHe Rd."),				Shelter_t(30.6264,104.0945,"LongZhou Rd."),			Shelter_t(30.6423,104.0921,"WangJiang Rd.") };//Shelters' location
