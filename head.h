#define _CRT_SECURE_NO_WARNINGS 1
#include "easyx.h"
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include <windows.h>//����
#pragma comment(lib,"winmm.lib")
#include <Mmsystem.h>//����
#include<string.h>//�ַ�������
int menu();
int showRank();
void ranking(int);
void intsnake();//��ʼ����
void drawsnake();//���ߺ���ʾ����
void movesnake();//�ƶ���
void keysnake();//��������
void intfood();//��ʼ��ʳ��
void drawfood();//��ʳ��
void eatfood();//��ʳ��
int snakedie();//������
int gameview();//��ʼ��Ϸ����
int Asist();//��������
void setView();//����
void sort(int[], char[][50]);//��������
bool isHit(ExMessage* m, int x1, int y1, int x2, int y2);//ѡ��ѡ��