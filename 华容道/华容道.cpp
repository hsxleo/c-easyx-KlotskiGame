#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <comdef.h>
#include <math.h>

//菜单窗口的高和宽
#define MENU_HEIGHT 600   
#define MENU_WIDTH 800  

//游戏窗口的高和宽
#define GAME_HEIGHT 600
#define GAME_WIDTH 700
//
////游戏人物图片的大小
//#define PEOPLE 80

//图片变量
IMAGE menuBackground;  //菜单背景图
IMAGE gameBackground;  //游戏背景图

//游戏图纸
/*
* 0 --> 空地
* 1 --> 小兵
* 2 --> 曹操
* 3 --> 关羽
* 4 --> 张飞
* 5 --> 赵云
* 6 --> 马超
* 7 --> 黄忠
*/
char name[8][8] = { "","卒","曹操","关羽","张飞","赵云","马超","黄忠" };

int map[3][7][6] = {
	{
	{-1,-1,-1,-1,-1,-1},
	{-1,1,2,2,1,-1},
	{-1,1,2,2,1,-1},
	{-1,3,3,4,4,-1},
	{-1,5,5,6,6,-1},
	{-1,0,7,7,0,-1},
	{-1,-1,-1,-1,-1,-1},
	},
	{
	{-1,-1,-1,-1,-1,-1},
	{-1,1,2,2,7,-1},
	{-1,1,2,2,7,-1},
	{-1,3,3,4,4,-1},
	{-1,5,5,6,6,-1},
	{-1,1,0,0,1,-1},
	{-1,-1,-1,-1,-1,-1},
	},
	{
	{-1,-1,-1,-1,-1,-1},
	{-1,6,2,2,7,-1},
	{-1,6,2,2,7,-1},
	{-1,1,3,3,1,-1},
	{-1,1,4,4,1,-1},
	{-1,0,5,5,0,-1},
	{-1,-1,-1,-1,-1,-1},
	},
};

int map_arr[3][7][6];  //用来操作的数组
int game_flg;  //当前游戏

int click_flg;    //0为卒 1为曹操 2为横五虎 3为纵五虎
int click_i, click_j;  //点击图块的左上角的方块坐标

int step = 0;


void loadResource();//加载资源
void drawMenu();//绘制菜单
void mouseMenu();//菜单鼠标交互
void playGame(int arr[][6]); //玩游戏
void drawGame(int arr[][6]);//绘制游戏
bool isWin(int arr[][6]);  //判断游戏是否胜利
void click(int arr[][6], int i, int j);//点击的方块

//加载资源
void loadResource() {
	loadimage(&menuBackground, "image\\menubackground.jpg", MENU_WIDTH, MENU_HEIGHT);
	loadimage(&gameBackground, "image\\gamebackground.jpg", GAME_WIDTH, GAME_HEIGHT);
}

//数组初始化
void arrInit() {
	int i, j, k;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 7; j++) {
			for (k = 0; k < 6; k++) {
				map_arr[i][j][k] = map[i][j][k];
			}
		}
	}
}

//绘制菜单
void drawMenu() {
	//生成画布
	initgraph(MENU_WIDTH, MENU_HEIGHT);
	//菜单背景图片
	putimage(0, 0, &menuBackground);

	settextcolor(RED);          //设置字体颜色
	settextstyle(40, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	outtextxy(400, 100, "过五关");
	outtextxy(400, 180, "水泄不通");
	outtextxy(400, 260, "层层设防");
	outtextxy(400, 340, "退出游戏");
}


//菜单鼠标交互
void mouseMenu() {
	arrInit();  //初始化游戏数组
	drawMenu();      //绘制菜单界面
	MOUSEMSG m;  //鼠标变量   用来获取鼠标信息

	while (true) {
		//获取鼠标位置
		m = GetMouseMsg();

		//判断鼠标位置
		if (m.x >= 400 && m.x <= 520 && m.y >= 100 && m.y <= 140) {  
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				game_flg = 0;
				playGame(map_arr[0]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 180 && m.y <= 220) {  
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				game_flg = 1;
				playGame(map_arr[1]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 260 && m.y <= 300) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				game_flg = 2;
				playGame(map_arr[2]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 340 && m.y <= 380) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				exit(0);  //退出游戏
			}
		}
	}
}

//绘制游戏
void drawGame(int arr[][6]) {
	//清空画布
	cleardevice();
	//游戏背景图片
	putimage(0, 0, &gameBackground);

	//颜色 样式
	setcolor(BLACK);
	settextcolor(RED);          //设置字体颜色
	settextstyle(20, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明

	//外边框
	rectangle(250, 150, 450, 400);
	char s[8];
	//绘制华容道
	int i, j;
	for (i = 1; i <= 5; i++) {
		for (j = 1; j <= 4; j++) {
			if (arr[i][j] == 1) {  //小卒
				rectangle( 200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 15,"卒");
			}
			else if (arr[i][j] == 2 && arr[i][j] == arr[i][j + 1] && arr[i][j] == arr[i + 1][j]) {  //曹操
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 2) * 50);
				wsprintf(s, "%s", name[arr[i][j]]);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 40,  name[arr[i][j]]);
			}
			else if (arr[i][j] >= 3 && arr[i][j] == arr[i][j + 1]) {  //横  五虎上将
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 15, name[arr[i][j]]);
			}
			else if (arr[i][j] >= 3 && arr[i][j] == arr[i + 1][j]) {  //纵  五虎上将
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 2) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 30, name[arr[i][j]]);
			}
		}
	}

	//重新开始  返回菜单
	settextcolor(BLACK);          //设置字体颜色
	settextstyle(30, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	char s2[10];
	sprintf(s2, "步数:%d", step);
	outtextxy(510, 200, s2);
	outtextxy(500, 400, "重新开始");
	outtextxy(500, 450, "返回菜单");
}

//玩游戏
void playGame(int arr[][6]) {

	//生成画布
	initgraph(GAME_WIDTH, GAME_HEIGHT);
	drawGame(arr);   //绘制游戏画面

	MOUSEMSG m;  //鼠标变量   用来获取鼠标信息
	MOUSEMSG mtmp;  //鼠标变量  临时变量
	int mouse_flg = 0;
	int n = 0;    //每两次交换一次
	while (true) {
		//获取鼠标位置
		m = GetMouseMsg();
		int i, j;
		if (m.x >= 500 && m.x <= 620 && m.y >= 400 && m.y <= 430) {  //重新开始
			if (m.uMsg == WM_LBUTTONDOWN) {
				arrInit();
				step = 0;
				playGame(map_arr[game_flg]);
			}
		}
		else if (m.x >= 500 && m.x <= 620 && m.y >= 450 && m.y <= 480) {  //返回菜单
			if (m.uMsg == WM_LBUTTONDOWN) {
				step = 0;
				mouseMenu();
			}
		}
		if (mouse_flg == 0) {
			for (i = 1; i <= 5; i++) {
				for (j = 1; j <= 4; j++) {
					if (m.x >= 200 + j * 50 && m.x <= 200 + (j + 1) * 50 && m.y >= 100 + i * 50 && m.y <= 100 + (i + 1) * 50) {
						if (m.uMsg == WM_LBUTTONDOWN) {
							mouse_flg = 1;
							//记住坐标点击的坐标
							mtmp.x = m.x;
							mtmp.y = m.y;
							//判断点击的方块
							click(arr, i, j);
							break;
						}
					}
				}
				if (mouse_flg == 1) {
					break;
				}
			}
		}


		//判断移动方向
		if (m.uMsg == WM_LBUTTONUP && mouse_flg == 1) {
			mouse_flg = 0;
			int tx = m.x - mtmp.x;
			int ty = m.y - mtmp.y;
			if (abs(tx) > abs(ty)) {  //水平移动
				if (tx > 0) {  //右移
					if (click_flg == 2 && arr[click_i][click_j + 2] == 0) {  
						arr[click_i][click_j + 2] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						step++;
					}
					else if (click_flg == 0 && arr[click_i][click_j + 1] == 0) { 
						arr[click_i][click_j + 1] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						step++;
					}
					else if (click_flg == 3 && arr[click_i][click_j + 1] == 0 && arr[click_i + 1][click_j + 1] == 0) {
						arr[click_i][click_j + 1] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						arr[click_i + 1][click_j + 1] = arr[click_i + 1][click_j];
						arr[click_i + 1][click_j] = 0;
						step++;
					}
					else if (click_flg == 1 && arr[click_i][click_j + 2] == 0 && arr[click_i + 1][click_j + 2] == 0) {
						arr[click_i][click_j + 2] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						arr[click_i + 1][click_j + 2] = arr[click_i + 1][click_j + 1];
						arr[click_i + 1][click_j + 1] = arr[click_i + 1][click_j];
						arr[click_i + 1][click_j] = 0;
						step++;
					}
				}
				else {  //左移
					if (click_flg == 0 && arr[click_i][click_j - 1] == 0) {  
						arr[click_i][click_j - 1] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						step++;
					}
					else if (click_flg == 2 && arr[click_i][click_j - 1] == 0) { 
						arr[click_i][click_j - 1] = arr[click_i][click_j];
						arr[click_i][click_j] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = 0;
						step++;
					}
					else if (click_flg == 3 && arr[click_i][click_j - 1] == 0 && arr[click_i + 1][click_j - 1] == 0) {
						arr[click_i][click_j - 1] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						arr[click_i + 1][click_j - 1] = arr[click_i + 1][click_j];
						arr[click_i + 1][click_j] = 0;
						step++;
					}
					else if (click_flg == 1 && arr[click_i][click_j - 1] == 0 && arr[click_i + 1][click_j - 1] == 0) {
						arr[click_i][click_j - 1] = arr[click_i][click_j];
						arr[click_i][click_j] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = 0;
						arr[click_i+1][click_j - 1] = arr[click_i+1][click_j];
						arr[click_i+1][click_j] = arr[click_i+1][click_j + 1];
						arr[click_i+1][click_j + 1] = 0;
						step++;
					}
				}
			}
			else {  //竖直移动
				if (ty > 0) {  //下移
					if (click_flg == 0 && arr[click_i + 1][click_j] == 0) {
						arr[click_i + 1][click_j] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						step++;
					}
					else if (click_flg == 3 && arr[click_i + 2][click_j] == 0) {
						arr[click_i + 2][click_j] = arr[click_i + 1][click_j];
						arr[click_i + 1][click_j] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						step++;
					}
					else if (click_flg == 2 && arr[click_i + 1][click_j] == 0 && arr[click_i + 1][click_j + 1] == 0) {
						arr[click_i + 1][click_j] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						arr[click_i + 1][click_j + 1] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = 0;
						step++;
					}
					else if (click_flg == 1 && arr[click_i + 2][click_j] == 0 && arr[click_i + 2][click_j + 1] == 0) {
						arr[click_i + 2][click_j] = arr[click_i + 1][click_j];
						arr[click_i + 1][click_j] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						arr[click_i + 2][click_j + 1] = arr[click_i + 1][click_j + 1];
						arr[click_i + 1][click_j + 1] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = 0;
						step++;
					}
				}
				else {  //上移
					if (click_flg == 0 && arr[click_i - 1][click_j] == 0) {
						arr[click_i - 1][click_j] = arr[click_i][click_j]; 
						arr[click_i][click_j] = 0;
						step++;
					}
					else if (click_flg == 3 && arr[click_i - 1][click_j] == 0) {
						arr[click_i - 1][click_j] = arr[click_i][click_j];
						arr[click_i][click_j] = arr[click_i + 1][click_j];
						arr[click_i + 1][click_j] = 0;
						step++;
					}
					else if (click_flg == 2 && arr[click_i - 1][click_j] == 0 && arr[click_i - 1][click_j + 1] == 0) {
						arr[click_i - 1][click_j] = arr[click_i][click_j];
						arr[click_i][click_j] = 0;
						arr[click_i - 1][click_j + 1] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = 0;
						step++;
					}
					else if (click_flg == 1 && arr[click_i - 1][click_j] == 0 && arr[click_i - 1][click_j + 1] == 0) {
						arr[click_i - 1][click_j] = arr[click_i][click_j];
						arr[click_i][click_j] = arr[click_i + 1][click_j];
						arr[click_i + 1][click_j] = 0;
						arr[click_i - 1][click_j + 1] = arr[click_i][click_j + 1];
						arr[click_i][click_j + 1] = arr[click_i + 1][click_j + 1];
						arr[click_i + 1][click_j + 1] = 0;
						step++;
					}
				}
			}
			BeginBatchDraw();  //双缓冲绘图
			drawGame(arr);
			EndBatchDraw();
			if (isWin(arr)) {
				setcolor(BLACK);
				settextcolor(RED);          //设置字体颜色
				settextstyle(40, 0, "华文新魏"); //设置文字样式
				setbkmode(TRANSPARENT);       //文字背景透明
				outtextxy(250, 60, "游戏胜利！");
			}
		}
		
	}
}

//点击的方块
void click(int arr[][6], int i, int j) {
	//卒
	if (arr[i][j] == 1) {
		click_flg = 0;
		click_i = i;
		click_j = j;
	}
	//曹操
	else if (arr[i][j] == arr[i - 1][j - 1]) {  //左上
		click_flg = 1;
		click_i = i - 1;
		click_j = j - 1;
	}
	else if (arr[i][j] == arr[i - 1][j + 1]) {  //右上
		click_flg = 1;
		click_i = i - 1;
		click_j = j;
	}
	else if (arr[i][j] == arr[i + 1][j - 1]) {  //左下
		click_flg = 1;
		click_i = i;
		click_j = j - 1;
	}
	else if (arr[i][j] == arr[i + 1][j - 1]) {  //右下
		click_flg = 1;
		click_i = i;
		click_j = j;
	}
	//横 五虎
	else if (arr[i][j] == arr[i][j - 1]) {   //左
		click_flg = 2;
		click_i = i;
		click_j = j - 1;
	}
	else if (arr[i][j] == arr[i][j + 1]) {   //右
		click_flg = 2;
		click_i = i;
		click_j = j;
	}
	//纵 五虎
	else if (arr[i][j] == arr[i - 1][j]) {   //上
		click_flg = 3;
		click_i = i - 1;
		click_j = j;
	}
	else if (arr[i][j] == arr[i + 1][j]) {   //下
		click_flg = 3;
		click_i = i;
		click_j = j;
	}
}

//判断是否胜利
bool isWin(int arr[][6]) {
	if (arr[5][2] == 2 && arr[5][3] == 2) {  //曹操在出口
		return true;
	}
	return false;
}


int main() {
	loadResource();
	mouseMenu();
	_getch();
}