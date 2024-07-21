#include <graphics.h>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <windows.h>

using namespace std;

const int WIDTH = 1000;
const int HEIGHT = 800;
const int BLOCK = 10;
const int numX = 100;
const int numY = 80;

#define FLOOR 0
#define WALL 1
#define WAIT 2

#define nothing COLORREF RGB(255, 255, 255)
#define wall COLORREF RGB(50, 50, 50)
#define wait COLORREF RGB(0, 255, 0)
#define selected COLORREF RGB(255, 0, 0)

struct position {
	int x;
	int y;
	position(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

int map[numX][numY];
vector<position> selectedVec;
vector<position> Vec;

void init();
void drawPos(int x, int y, COLORREF RGB);
void drawMap();
void addVec(int x, int y);
void primLoop();
void breakWall(int x, int y);

int main() {
	//设置随机种子，生成随机数
	srand((unsigned)time(0));
	//初始化窗口
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();//开始批量绘图
	cleardevice();
	init();
	drawMap();
	system("pause");
	closegraph();
}

//根据RGB在x，y位置绘制矩形
void drawPos(int x, int y, COLORREF RGB) {
	setfillcolor(RGB);
	setlinecolor(COLORREF RGB(0, 0, 0));
	fillrectangle(x * BLOCK, y * BLOCK, BLOCK + x * BLOCK, y * BLOCK + BLOCK);
}

//根据Map数组绘制整个地图
void drawMap() {
	for (int i = 0; i < numX; i++) {
		for (int j = 0; j < numY; j++) {
			if (map[i][j] == WALL) {
				drawPos(i, j, wall);
			}else if (map[i][j] == FLOOR) {
				drawPos(i, j, nothing);
			}else if (map[i][j] == WAIT) {
				drawPos(i, j, wait);
			}
		}
	}
	FlushBatchDraw();
}

void init() {
	for (int i = 0; i < numX; i++) {
		for (int j = 0; j < numY; j++) {
			map[i][j] = WALL;
		}
	}
	map[0][0] = FLOOR;
	//初始化待选数组
	addVec(0, 0);
	//进行prim随机算法生成迷宫
	primLoop();
}

//把（x, y）位置周围原本是路点的墙添加到待选列表中
void addVec(int x, int y) {
	//上面的点
	if (y - 2 >= 0 && map[x][y - 2] == WALL) {
		position pos(x, y - 2);
		selectedVec.push_back(pos);
		map[x][y - 2] = WAIT;
	}
	//下面的点
	if (y + 2 < numY && map[x][y + 2] == WALL) {
		position pos(x, y + 2);
		selectedVec.push_back(pos);
		map[x][y + 2] = WAIT;
	}
	//左边的点
	if (x - 2 >= 0 && map[x - 2][y] == WALL) {
		position pos(x - 2, y);
		selectedVec.push_back(pos);
		map[x - 2][y] = WAIT;
	}
	//右边的点
	if (x + 2 < numX && map[x + 2][y] == WALL) {
		position pos(x + 2, y);
		selectedVec.push_back(pos);
		map[x + 2][y] = WAIT;
	}
}

void primLoop() {
	while (!selectedVec.empty()) {
		//随机从待选列表中选择一个路点
		int randomIndex = rand() % selectedVec.size();
		position pos = selectedVec[randomIndex];
		drawPos(pos.x, pos.y, selected);
		//选择好路点之后，将路点随机与周围已经是路的一个路点打通
		breakWall(pos.x, pos.y);
		map[pos.x][pos.y] = FLOOR;
		addVec(pos.x, pos.y);
		selectedVec.erase(selectedVec.begin() + randomIndex);
		//批量绘图
		FlushBatchDraw();
		//Sleep(10);
		drawMap();
	}
	//selectedVec.clear();
}

void breakWall(int x, int y) {
	Vec.clear();
	//上方有路点
	if (y - 2 >= 0 && map[x][y - 2] == FLOOR) {
		position pos(x, y - 1);
		Vec.push_back(pos);
	}
	//下方有路点
	if (y + 2 < numY && map[x][y + 2] == FLOOR) {
		position pos(x, y + 1);
		Vec.push_back(pos);
	}
	//左边有路点
	if (x - 2 >= 0 && map[x - 2][y] == FLOOR) {
		position pos(x - 1, y);
		Vec.push_back(pos);
	}
	//右边有路点
	if (x + 2 < numX && map[x + 2][y] == FLOOR) {
		position pos(x + 1, y);
		Vec.push_back(pos);
	}
	int randomIndex = rand() % Vec.size();
	position pos = Vec[randomIndex];
	//将两点打通
	map[pos.x][pos.y] = FLOOR;
}