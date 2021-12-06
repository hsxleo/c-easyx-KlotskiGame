#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>

//�˵����ڵĸߺͿ�
#define MENU_HEIGHT 600   
#define MENU_WIDTH 800  

//��Ϸ���ڵĸߺͿ�
#define GAME_HEIGHT 600
#define GAME_WIDTH 700

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

int map2[3][7][6];  //��������������
int gFlg;  //��ǰ��Ϸ

int cFlg;    //0Ϊ�� 1Ϊ�ܲ� 2Ϊ���廢 3Ϊ���廢
int cx, cy;  //���ͼ������Ͻǵķ�������


void menuDraw();//���Ʋ˵�
void mouseMenu();//�˵���꽻��
void gamePlay(int a[][6]); //����Ϸ
void gameDraw(int a[][6]);//������Ϸ
void click(int a[][6], int i, int j);//����ķ���
void aInit();  //��ʼ����Ϸ����



int main() {
	//������Դ
	loadimage(&menuBackground, "image\\menubackground.jpg", MENU_WIDTH, MENU_HEIGHT);
	loadimage(&gameBackground, "image\\gamebackground.jpg", GAME_WIDTH, GAME_HEIGHT);
	mouseMenu();
}

//���Ʋ˵�
void menuDraw() {
	//���ɻ���
	initgraph(MENU_WIDTH, MENU_HEIGHT);
	//�˵�����ͼƬ
	putimage(0, 0, &menuBackground);

	settextcolor(RED);          //����������ɫ
	settextstyle(40, 0, "��������"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��
	outtextxy(400, 100, "�����");
	outtextxy(400, 180, "ˮй��ͨ");
	outtextxy(400, 260, "������");
	outtextxy(400, 340, "�˳���Ϸ");
}

//�˵���꽻��
void mouseMenu() {
	aInit();  //��ʼ����Ϸ����
	menuDraw();      //���Ʋ˵�����
	MOUSEMSG m;  //������   ������ȡ�����Ϣ

	while (true) {
		//��ȡ���λ��
		m = GetMouseMsg();

		//�ж����λ��
		if (m.x >= 400 && m.x <= 520 && m.y >= 100 && m.y <= 140) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				gFlg = 0;
				gamePlay(map2[0]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 180 && m.y <= 220) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				gFlg = 1;
				gamePlay(map2[1]);
			}
		}
		else if (m.x >= 400 && m.y <= 560 && m.y >= 260 && m.y <= 300) {
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				gFlg = 2;
				gamePlay(map2[2]);
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
void gameDraw(int a[][6]) {
	//��ջ���
	cleardevice();
	//��Ϸ����ͼƬ
	putimage(0, 0, &gameBackground);

	//��ɫ ��ʽ
	setcolor(BLACK);
	settextcolor(GREEN);          //����������ɫ
	settextstyle(20, 0, "����"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��

	//��߿�
	rectangle(250, 150, 450, 400);
	char s[8];
	//���ƻ��ݵ�
	int i, j;
	for (i = 1; i <= 5; i++) {
		for (j = 1; j <= 4; j++) {
			if (a[i][j] == 1) {  //С��
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 15, "��");
			}
			else if (a[i][j] == 2 && a[i][j] == a[i][j + 1] && a[i][j] == a[i + 1][j]) {  //�ܲ�
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 2) * 50);
				wsprintf(s, "%s", name[a[i][j]]);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 40, name[a[i][j]]);
			}
			else if (a[i][j] >= 3 && a[i][j] == a[i][j + 1]) {  //��  �廢�Ͻ�
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 2) * 50, 100 + (i + 1) * 50);
				outtextxy(200 + j * 50 + 30, 100 + i * 50 + 15, name[a[i][j]]);
			}
			else if (a[i][j] >= 3 && a[i][j] == a[i + 1][j]) {  //��  �廢�Ͻ�
				rectangle(200 + j * 50, 100 + i * 50, 200 + (j + 1) * 50, 100 + (i + 2) * 50);
				outtextxy(200 + j * 50 + 15, 100 + i * 50 + 30, name[a[i][j]]);
			}
		}
	}

	//���¿�ʼ  ���ز˵�
	settextcolor(GREEN);          //����������ɫ
	settextstyle(30, 0, "΢���ź�"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��
	outtextxy(230, 460, "���¿�ʼ");
	outtextxy(400, 460, "���ز˵�");
}

//����Ϸ
void gamePlay(int a[][6]) {

	//���ɻ���
	initgraph(GAME_WIDTH, GAME_HEIGHT);
	gameDraw(a);   //������Ϸ����

	MOUSEMSG m;  //������   ������ȡ�����Ϣ
	MOUSEMSG mtmp;  //������  ��ʱ����
	int mouse_flg = 0;
	int n = 0;    //ÿ���ν���һ��
	while (true) {
		//��ȡ���λ��
		m = GetMouseMsg();
		int i, j;
		if (m.x >= 230 && m.x <= 350 && m.y >= 460 && m.y <= 490) {  //���¿�ʼ
			if (m.uMsg == WM_LBUTTONDOWN) {
				aInit();
				gamePlay(map2[gFlg]);
			}
		}
		else if (m.x >= 400 && m.x <= 520 && m.y >= 460 && m.y <= 490) {  //���ز˵�
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
							//��ס������������
							mtmp.x = m.x;
							mtmp.y = m.y;
							//�жϵ���ķ���
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


		//�ж��ƶ�����
		if (m.uMsg == WM_LBUTTONUP && mouse_flg == 1) {
			mouse_flg = 0;
			int tx = m.x - mtmp.x;
			int ty = m.y - mtmp.y;
			if (abs(tx) > abs(ty)) {  //ˮƽ�ƶ�
				if (tx > 0) {  //����
					if (cFlg == 2 && a[cx][cy + 2] == 0) {     //�� �廢    �ж��ұ�һ�����ǲ��ǿյ� ���������������ƶ�һ��  Ȼ����ߵĸ��Ϊ�յ�
						a[cx][cy + 2] = a[cx][cy + 1];
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 0 && a[cx][cy + 1] == 0) {  //��   �ж��ұ�һ�����ǲ��ǿյ� ���������ƶ�һ�� Ȼ����ߵĸ��Ϊ�յ�
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
					}
					else if (cFlg == 3 && a[cx][cy + 1] == 0 && a[cx + 1][cy + 1] == 0) {  //�� �廢 �ж��ұ��������ڵ�  ���ƶ�1
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx + 1][cy + 1] = a[cx + 1][cy];
						a[cx + 1][cy] = 0;
					}
					else if (cFlg == 1 && a[cx][cy + 2] == 0 && a[cx + 1][cy + 2] == 0) {   //�ܲ�  �ж��ұ��������ڵ�  ���ƶ�2
						a[cx][cy + 2] = a[cx][cy + 1];
						a[cx][cy + 1] = a[cx][cy];
						a[cx][cy] = 0;
						a[cx + 1][cy + 2] = a[cx + 1][cy + 1];
						a[cx + 1][cy + 1] = a[cx + 1][cy];
						a[cx + 1][cy] = 0;
					}
				}
				else {  //����
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
			else {  //��ֱ�ƶ�
				if (ty > 0) {  //����
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
				else {  //����
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
			if (a[5][2] == 2 && a[5][3] == 2) {  //�ж��Ƿ�ʤ��
				setcolor(BLACK);
				settextcolor(RED);          //����������ɫ
				settextstyle(40, 0, "������κ"); //����������ʽ
				setbkmode(TRANSPARENT);       //���ֱ���͸��
				outtextxy(250, 60, "��Ϸʤ����");
			}
		}

	}
}

//�����ʼ��
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

//����ķ���
void click(int a[][6], int i, int j) {
	//��
	if (a[i][j] == 1) {
		cFlg = 0;
		cx = i;
		cy = j;
	}
	//�ܲ�
	else if (a[i][j] == a[i - 1][j - 1]) {  //����
		cFlg = 1;
		cx = i - 1;
		cy = j - 1;
	}
	else if (a[i][j] == a[i - 1][j + 1]) {  //����
		cFlg = 1;
		cx = i - 1;
		cy = j;
	}
	else if (a[i][j] == a[i + 1][j - 1]) {  //����
		cFlg = 1;
		cx = i;
		cy = j - 1;
	}
	else if (a[i][j] == a[i + 1][j - 1]) {  //����
		cFlg = 1;
		cx = i;
		cy = j;
	}
	//�� �廢
	else if (a[i][j] == a[i][j - 1]) {   //��
		cFlg = 2;
		cx = i;
		cy = j - 1;
	}
	else if (a[i][j] == a[i][j + 1]) {   //��
		cFlg = 2;
		cx = i;
		cy = j;
	}
	//�� �廢
	else if (a[i][j] == a[i - 1][j]) {   //��
		cFlg = 3;
		cx = i - 1;
		cy = j;
	}
	else if (a[i][j] == a[i + 1][j]) {   //��
		cFlg = 3;
		cx = i;
		cy = j;
	}
}
