#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <comdef.h>
#include <math.h>

//�˵����ڵĸߺͿ�
#define MENU_HEIGHT 600   
#define MENU_WIDTH 800  

//��Ϸ���ڵĸߺͿ�
#define GAME_HEIGHT 600
#define GAME_WIDTH 700
//
////��Ϸ����ͼƬ�Ĵ�С
//#define PEOPLE 80

//ͼƬ����
IMAGE menuBackground;  //�˵�����ͼ
IMAGE gameBackground;  //��Ϸ����ͼ

//��Ϸͼֽ
/*
* 0 --> �յ�
* 1 --> С��
* 2 --> �ܲ�
* 3 --> ����
* 4 --> �ŷ�
* 5 --> ����
* 6 --> ��
* 7 --> ����
*/
char name[8][8] = { "","��","�ܲ�","����","�ŷ�","����","��","����" };

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

int map_arr[3][7][6];  //��������������
int game_flg;  //��ǰ��Ϸ

int click_flg;    //0Ϊ�� 1Ϊ�ܲ� 2Ϊ���廢 3Ϊ���廢
int click_i, click_j;  //���ͼ������Ͻǵķ�������

int step = 0;


void loadResource();//������Դ
void drawMenu();//���Ʋ˵�
void mouseMenu();//�˵���꽻��
void playGame(int arr[][6]); //����Ϸ
void drawGame(int arr[][6]);//������Ϸ
bool isWin(int arr[][6]);  //�ж���Ϸ�Ƿ�ʤ��
void click(int arr[][6], int i, int j);//����ķ���

//������Դ
void loadResource() {
	loadimage(&menuBackground, "image\\menubackground.jpg", MENU_WIDTH, MENU_HEIGHT);
	loadimage(&gameBackground, "image\\gamebackground.jpg", GAME_WIDTH, GAME_HEIGHT);
}

//�����ʼ��
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

//���Ʋ˵�
void drawMenu() {
	//���ɻ���
	initgraph(MENU_WIDTH, MENU_HEIGHT);
	//�˵�����ͼƬ
	putimage(0, 0, &menuBackground);

	settextcolor(RED);          //����������ɫ
	settextstyle(40, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��
	outtextxy(400, 100, "�����");
	outtextxy(400, 180, "ˮй��ͨ");
	outtextxy(400, 260, "������");
	outtextxy(400, 340, "�˳���Ϸ");
}


//�˵���꽻��
void mouseMenu() {
	arrInit();  //��ʼ����Ϸ����
	drawMenu();      //���Ʋ˵�����
	MOUSEMSG m;  //������   ������ȡ�����Ϣ

	while (true) {
		//��ȡ���λ��
		m = GetMouseMsg();

		//�ж����λ��
		if (m.x >= 400 && m.x <= 520 && m.y >= 100 && m.y <= 140) {  
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				game_flg = 0;
				playGame(map_arr[0]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 180 && m.y <= 220) {  
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				game_flg = 1;
				playGame(map_arr[1]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 260 && m.y <= 300) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				game_flg = 2;
				playGame(map_arr[2]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 340 && m.y <= 380) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				exit(0);  //�˳���Ϸ
			}
		}
	}
}

//������Ϸ
void drawGame(int arr[][6]) {
	//��ջ���
	cleardevice();
	//��Ϸ����ͼƬ
	putimage(0, 0, &gameBackground);

	//��ɫ ��ʽ
	setcolor(BLACK);
	settextcolor(RED);          //����������ɫ
	settextstyle(20, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��

	//��߿�
	rectangle(250, 150, 450, 400);
	char s[8];
	//���ƻ��ݵ�
	int i, j;
	for (i = 1; i <= 5; i++) {
		for (j = 1; j <= 4; j++) {
			if (arr[i][j] == 1) {  //С��
				rectangle( 200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 15,"��");
			}
			else if (arr[i][j] == 2 && arr[i][j] == arr[i][j + 1] && arr[i][j] == arr[i + 1][j]) {  //�ܲ�
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 2) * 50);
				wsprintf(s, "%s", name[arr[i][j]]);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 40,  name[arr[i][j]]);
			}
			else if (arr[i][j] >= 3 && arr[i][j] == arr[i][j + 1]) {  //��  �廢�Ͻ�
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 15, name[arr[i][j]]);
			}
			else if (arr[i][j] >= 3 && arr[i][j] == arr[i + 1][j]) {  //��  �廢�Ͻ�
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 2) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 30, name[arr[i][j]]);
			}
		}
	}

	//���¿�ʼ  ���ز˵�
	settextcolor(BLACK);          //����������ɫ
	settextstyle(30, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��
	char s2[10];
	sprintf(s2, "����:%d", step);
	outtextxy(510, 200, s2);
	outtextxy(500, 400, "���¿�ʼ");
	outtextxy(500, 450, "���ز˵�");
}

//����Ϸ
void playGame(int arr[][6]) {

	//���ɻ���
	initgraph(GAME_WIDTH, GAME_HEIGHT);
	drawGame(arr);   //������Ϸ����

	MOUSEMSG m;  //������   ������ȡ�����Ϣ
	MOUSEMSG mtmp;  //������  ��ʱ����
	int mouse_flg = 0;
	int n = 0;    //ÿ���ν���һ��
	while (true) {
		//��ȡ���λ��
		m = GetMouseMsg();
		int i, j;
		if (m.x >= 500 && m.x <= 620 && m.y >= 400 && m.y <= 430) {  //���¿�ʼ
			if (m.uMsg == WM_LBUTTONDOWN) {
				arrInit();
				step = 0;
				playGame(map_arr[game_flg]);
			}
		}
		else if (m.x >= 500 && m.x <= 620 && m.y >= 450 && m.y <= 480) {  //���ز˵�
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
							//��ס������������
							mtmp.x = m.x;
							mtmp.y = m.y;
							//�жϵ���ķ���
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


		//�ж��ƶ�����
		if (m.uMsg == WM_LBUTTONUP && mouse_flg == 1) {
			mouse_flg = 0;
			int tx = m.x - mtmp.x;
			int ty = m.y - mtmp.y;
			if (abs(tx) > abs(ty)) {  //ˮƽ�ƶ�
				if (tx > 0) {  //����
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
				else {  //����
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
			else {  //��ֱ�ƶ�
				if (ty > 0) {  //����
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
				else {  //����
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
			BeginBatchDraw();  //˫�����ͼ
			drawGame(arr);
			EndBatchDraw();
			if (isWin(arr)) {
				setcolor(BLACK);
				settextcolor(RED);          //����������ɫ
				settextstyle(40, 0, "������κ"); //����������ʽ
				setbkmode(TRANSPARENT);       //���ֱ���͸��
				outtextxy(250, 60, "��Ϸʤ����");
			}
		}
		
	}
}

//����ķ���
void click(int arr[][6], int i, int j) {
	//��
	if (arr[i][j] == 1) {
		click_flg = 0;
		click_i = i;
		click_j = j;
	}
	//�ܲ�
	else if (arr[i][j] == arr[i - 1][j - 1]) {  //����
		click_flg = 1;
		click_i = i - 1;
		click_j = j - 1;
	}
	else if (arr[i][j] == arr[i - 1][j + 1]) {  //����
		click_flg = 1;
		click_i = i - 1;
		click_j = j;
	}
	else if (arr[i][j] == arr[i + 1][j - 1]) {  //����
		click_flg = 1;
		click_i = i;
		click_j = j - 1;
	}
	else if (arr[i][j] == arr[i + 1][j - 1]) {  //����
		click_flg = 1;
		click_i = i;
		click_j = j;
	}
	//�� �廢
	else if (arr[i][j] == arr[i][j - 1]) {   //��
		click_flg = 2;
		click_i = i;
		click_j = j - 1;
	}
	else if (arr[i][j] == arr[i][j + 1]) {   //��
		click_flg = 2;
		click_i = i;
		click_j = j;
	}
	//�� �廢
	else if (arr[i][j] == arr[i - 1][j]) {   //��
		click_flg = 3;
		click_i = i - 1;
		click_j = j;
	}
	else if (arr[i][j] == arr[i + 1][j]) {   //��
		click_flg = 3;
		click_i = i;
		click_j = j;
	}
}

//�ж��Ƿ�ʤ��
bool isWin(int arr[][6]) {
	if (arr[5][2] == 2 && arr[5][3] == 2) {  //�ܲ��ڳ���
		return true;
	}
	return false;
}


int main() {
	loadResource();
	mouseMenu();
	_getch();
}