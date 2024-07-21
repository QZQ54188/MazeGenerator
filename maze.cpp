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
	//����������ӣ����������
	srand((unsigned)time(0));
	//��ʼ������
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();//��ʼ������ͼ
	cleardevice();
	init();
	drawMap();
	system("pause");
	closegraph();
}

//����RGB��x��yλ�û��ƾ���
void drawPos(int x, int y, COLORREF RGB) {
	setfillcolor(RGB);
	setlinecolor(COLORREF RGB(0, 0, 0));
	fillrectangle(x * BLOCK, y * BLOCK, BLOCK + x * BLOCK, y * BLOCK + BLOCK);
}

//����Map�������������ͼ
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
	//��ʼ����ѡ����
	addVec(0, 0);
	//����prim����㷨�����Թ�
	primLoop();
}

//�ѣ�x, y��λ����Χԭ����·���ǽ��ӵ���ѡ�б���
void addVec(int x, int y) {
	//����ĵ�
	if (y - 2 >= 0 && map[x][y - 2] == WALL) {
		position pos(x, y - 2);
		selectedVec.push_back(pos);
		map[x][y - 2] = WAIT;
	}
	//����ĵ�
	if (y + 2 < numY && map[x][y + 2] == WALL) {
		position pos(x, y + 2);
		selectedVec.push_back(pos);
		map[x][y + 2] = WAIT;
	}
	//��ߵĵ�
	if (x - 2 >= 0 && map[x - 2][y] == WALL) {
		position pos(x - 2, y);
		selectedVec.push_back(pos);
		map[x - 2][y] = WAIT;
	}
	//�ұߵĵ�
	if (x + 2 < numX && map[x + 2][y] == WALL) {
		position pos(x + 2, y);
		selectedVec.push_back(pos);
		map[x + 2][y] = WAIT;
	}
}

void primLoop() {
	while (!selectedVec.empty()) {
		//����Ӵ�ѡ�б���ѡ��һ��·��
		int randomIndex = rand() % selectedVec.size();
		position pos = selectedVec[randomIndex];
		drawPos(pos.x, pos.y, selected);
		//ѡ���·��֮�󣬽�·���������Χ�Ѿ���·��һ��·���ͨ
		breakWall(pos.x, pos.y);
		map[pos.x][pos.y] = FLOOR;
		addVec(pos.x, pos.y);
		selectedVec.erase(selectedVec.begin() + randomIndex);
		//������ͼ
		FlushBatchDraw();
		//Sleep(10);
		drawMap();
	}
	//selectedVec.clear();
}

void breakWall(int x, int y) {
	Vec.clear();
	//�Ϸ���·��
	if (y - 2 >= 0 && map[x][y - 2] == FLOOR) {
		position pos(x, y - 1);
		Vec.push_back(pos);
	}
	//�·���·��
	if (y + 2 < numY && map[x][y + 2] == FLOOR) {
		position pos(x, y + 1);
		Vec.push_back(pos);
	}
	//�����·��
	if (x - 2 >= 0 && map[x - 2][y] == FLOOR) {
		position pos(x - 1, y);
		Vec.push_back(pos);
	}
	//�ұ���·��
	if (x + 2 < numX && map[x + 2][y] == FLOOR) {
		position pos(x + 1, y);
		Vec.push_back(pos);
	}
	int randomIndex = rand() % Vec.size();
	position pos = Vec[randomIndex];
	//�������ͨ
	map[pos.x][pos.y] = FLOOR;
}