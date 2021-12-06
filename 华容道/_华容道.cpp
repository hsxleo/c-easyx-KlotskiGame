#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>

//菜单窗口的高和宽
#define MENU_HEIGHT 600   
#define MENU_WIDTH 800  

//游戏窗口的高和宽
#define GAME_HEIGHT 600
#define GAME_WIDTH 700

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

int map2[3][7][6];  //用来操作的数组
int gFlg;  //当前游戏

int cFlg;    //0为卒 1为曹操 2为横五虎 3为纵五虎
int cx, cy;  //点击图块的左上角的方块坐标


void menuDraw();//绘制菜单
void mouseMenu();//菜单鼠标交互
void gamePlay(int a[][6]); //玩游戏
void gameDraw(int a[][6]);//绘制游戏
void click(int a[][6], int i, int j);//点击的方块
void aInit();  //初始化游戏数组



int main() {
	//加载资源
	loadimage(&menuBackground, "image\\menubackground.jpg", MENU_WIDTH, MENU_HEIGHT);
	loadimage(&gameBackground, "image\\gamebackground.jpg", GAME_WIDTH, GAME_HEIGHT);
	mouseMenu();
}

//绘制菜单
void menuDraw() {
	//生成画布
	initgraph(MENU_WIDTH, MENU_HEIGHT);
	//菜单背景图片
	putimage(0, 0, &menuBackground);

	settextcolor(RED);          //设置字体颜色
	settextstyle(40, 0, "华文琥珀"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	outtextxy(400, 100, "过五关");
	outtextxy(400, 180, "水泄不通");
	outtextxy(400, 260, "层层设防");
	outtextxy(400, 340, "退出游戏");
}

//菜单鼠标交互
void mouseMenu() {
	aInit();  //初始化游戏数组
	menuDraw();      //绘制菜单界面
	MOUSEMSG m;  //鼠标变量   用来获取鼠标信息

	while (true) {
		//获取鼠标位置
		m = GetMouseMsg();

		//判断鼠标位置
		if (m.x >= 400 && m.x <= 520 && m.y >= 100 && m.y <= 140) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				gFlg = 0;
				gamePlay(map2[0]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 180 && m.y <= 220) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				gFlg = 1;
				gamePlay(map2[1]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 260 && m.y <= 300) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				gFlg = 2;
				gamePlay(map2[2]);
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
void gameDraw(int a[][6]) {
	//清空画布
	cleardevice();
	//游戏背景图片
	putimage(0, 0, &gameBackground);

	//颜色 样式
	setcolor(BLACK);
	settextcolor(GREEN);          //设置字体颜色
	settextstyle(20, 0, "宋体"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明

	//外边框
	rectangle(250, 150, 450, 400);
	char s[8];
	//绘制华容道
	int i, j;
	for (i = 1; i <= 5; i++) {
		for (j = 1; j <= 4; j++) {
			if (a[i][j] == 1) {  //小卒
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 15, "卒");
			}
			else if (a[i][j] == 2 && a[i][j] == a[i][j + 1] && a[i][j] == a[i + 1][j]) {  //曹操
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 2) * 50);
				wsprintf(s, "%s", name[a[i][j]]);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 40, name[a[i][j]]);
			}
			else if (a[i][j] >= 3 && a[i][j] == a[i][j + 1]) {  //横  五虎上将
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 15, name[a[i][j]]);
			}
			else if (a[i][j] >= 3 && a[i][j] == a[i + 1][j]) {  //纵  五虎上将
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 2) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 30, name[a[i][j]]);
			}
		}
	}

	//重新开始  返回菜单
	settextcolor(GREEN);          //设置字体颜色
	settextstyle(30, 0, "微软雅黑"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	outtextxy(230, 460, "重新开始");
	outtextxy(400, 460, "返回菜单");
}

//玩游戏
void gamePlay(int a[][6]) {

	//生成画布
	initgraph(GAME_WIDTH, GAME_HEIGHT);
	gameDraw(a);   //绘制游戏画面

	MOUSEMSG m;  //鼠标变量   用来获取鼠标信息
	MOUSEMSG mtmp;  //鼠标变量  临时变量
	int mouse_flg = 0;
	int n = 0;    //每两次交换一次
	while (true) {
		//获取鼠标位置
		m = GetMouseMsg();
		int i, j;
		if (m.x >= 230 && m.x <= 350 && m.y >= 460 && m.y <= 490) {  //重新开始
			if (m.uMsg == WM_LBUTTONDOWN) {
				aInit();
				gamePlay(map2[gFlg]);
			}
		}
		else if (m.x >= 400 && m.x <= 520 && m.y >= 460 && m.y <= 490) {  //返回菜单
			if (m.uMsg == WM_LBUTTONDOWN) {
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
							click(a, i, j);
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
					if (cFlg == 2 && a[cx][cy + 2] == 0) {     //横 五虎    判断右边一个格是不是空地 若是两个都向右移动一个  然后左边的格变为空地
						a[cx][cy + 2] = a[cx][cy + 1];
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 0 && a[cx][cy + 1] == 0) {  //卒   判断右边一个格是不是空地 若是向右移动一个 然后左边的格变为空地
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 3 && a[cx][cy + 1] == 0 && a[cx + 1][cy + 1] == 0) {  //纵 五虎 判断右边两个相邻的  右移动1
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx + 1][cy + 1] = a[cx + 1][cy];
						a[cx + 1][cy] = 0;
					}
					else if (cFlg == 1 && a[cx][cy + 2] == 0 && a[cx + 1][cy + 2] == 0) {   //曹操  判断右边两个相邻的  右移动2
						a[cx][cy + 2] = a[cx][cy + 1];
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx + 1][cy + 2] = a[cx + 1][cy + 1];
						a[cx + 1][cy + 1] = a[cx + 1][cy];
						a[cx + 1][cy] = 0;
					}
				}
				else {  //左移
					if (cFlg == 0 && a[cx][cy - 1] == 0) {
						a[cx][cy - 1] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 2 && a[cx][cy - 1] == 0) {
						a[cx][cy - 1] = a[cx][cy];
						a[cx][cy] = a[cx][cy + 1];
						a[cx][cy + 1] = 0;
					}
					else if (cFlg == 3 && a[cx][cy - 1] == 0 && a[cx + 1][cy - 1] == 0) {
						a[cx][cy - 1] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx + 1][cy - 1] = a[cx + 1][cy];
						a[cx + 1][cy] = 0;
					}
					else if (cFlg == 1 && a[cx][cy - 1] == 0 && a[cx + 1][cy - 1] == 0) {
						a[cx][cy - 1] = a[cx][cy];
						a[cx][cy] = a[cx][cy + 1];
						a[cx][cy + 1] = 0;
						a[cx + 1][cy - 1] = a[cx + 1][cy];
						a[cx + 1][cy] = a[cx + 1][cy + 1];
						a[cx + 1][cy + 1] = 0;
					}
				}
			}
			else {  //竖直移动
				if (ty > 0) {  //下移
					if (cFlg == 0 && a[cx + 1][cy] == 0) {
						a[cx + 1][cy] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 3 && a[cx + 2][cy] == 0) {
						a[cx + 2][cy] = a[cx + 1][cy];
						a[cx + 1][cy] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 2 && a[cx + 1][cy] == 0 && a[cx + 1][cy + 1] == 0) {
						a[cx + 1][cy] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx + 1][cy + 1] = a[cx][cy + 1];
						a[cx][cy + 1] = 0;
					}
					else if (cFlg == 1 && a[cx + 2][cy] == 0 && a[cx + 2][cy + 1] == 0) {
						a[cx + 2][cy] = a[cx + 1][cy];
						a[cx + 1][cy] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx + 2][cy + 1] = a[cx + 1][cy + 1];
						a[cx + 1][cy + 1] = a[cx][cy + 1];
						a[cx][cy + 1] = 0;
					}
				}
				else {  //上移
					if (cFlg == 0 && a[cx - 1][cy] == 0) {
						a[cx - 1][cy] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 3 && a[cx - 1][cy] == 0) {
						a[cx - 1][cy] = a[cx][cy];
						a[cx][cy] = a[cx + 1][cy];
						a[cx + 1][cy] = 0;
					}
					else if (cFlg == 2 && a[cx - 1][cy] == 0 && a[cx - 1][cy + 1] == 0) {
						a[cx - 1][cy] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx - 1][cy + 1] = a[cx][cy + 1];
						a[cx][cy + 1] = 0;
					}
					else if (cFlg == 1 && a[cx - 1][cy] == 0 && a[cx - 1][cy + 1] == 0) {
						a[cx - 1][cy] = a[cx][cy];
						a[cx][cy] = a[cx + 1][cy];
						a[cx + 1][cy] = 0;
						a[cx - 1][cy + 1] = a[cx][cy + 1];
						a[cx][cy + 1] = a[cx + 1][cy + 1];
						a[cx + 1][cy + 1] = 0;
					}
				}
			}
			gameDraw(a);
			if (a[5][2] == 2 && a[5][3] == 2) {  //判断是否胜利
				setcolor(BLACK);
				settextcolor(RED);          //设置字体颜色
				settextstyle(40, 0, "华文新魏"); //设置文字样式
				setbkmode(TRANSPARENT);       //文字背景透明
				outtextxy(250, 60, "游戏胜利！");
			}
		}

	}
}

//数组初始化
void aInit() {
	int i, j, k;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 7; j++) {
			for (k = 0; k < 6; k++) {
				map2[i][j][k] = map[i][j][k];
			}
		}
	}
}

//点击的方块
void click(int a[][6], int i, int j) {
	//卒
	if (a[i][j] == 1) {
		cFlg = 0;
		cx = i;
		cy = j;
	}
	//曹操
	else if (a[i][j] == a[i - 1][j - 1]) {  //左上
		cFlg = 1;
		cx = i - 1;
		cy = j - 1;
	}
	else if (a[i][j] == a[i - 1][j + 1]) {  //右上
		cFlg = 1;
		cx = i - 1;
		cy = j;
	}
	else if (a[i][j] == a[i + 1][j - 1]) {  //左下
		cFlg = 1;
		cx = i;
		cy = j - 1;
	}
	else if (a[i][j] == a[i + 1][j - 1]) {  //右下
		cFlg = 1;
		cx = i;
		cy = j;
	}
	//横 五虎
	else if (a[i][j] == a[i][j - 1]) {   //左
		cFlg = 2;
		cx = i;
		cy = j - 1;
	}
	else if (a[i][j] == a[i][j + 1]) {   //右
		cFlg = 2;
		cx = i;
		cy = j;
	}
	//纵 五虎
	else if (a[i][j] == a[i - 1][j]) {   //上
		cFlg = 3;
		cx = i - 1;
		cy = j;
	}
	else if (a[i][j] == a[i + 1][j]) {   //下
		cFlg = 3;
		cx = i;
		cy = j;
	}
}
