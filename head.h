#define _CRT_SECURE_NO_WARNINGS 1
#include "easyx.h"
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include <windows.h>//音乐
#pragma comment(lib,"winmm.lib")
#include <Mmsystem.h>//音乐
#include<string.h>//字符串函数
int menu();
int showRank();
void ranking(int);
void intsnake();//初始化蛇
void drawsnake();//画蛇和显示分数
void movesnake();//移动蛇
void keysnake();//按键控制
void intfood();//初始化食物
void drawfood();//画食物
void eatfood();//吃食物
int snakedie();//蛇死亡
int gameview();//开始游戏界面
int Asist();//帮助界面
void setView();//设置
void sort(int[], char[][50]);//分数排序
bool isHit(ExMessage* m, int x1, int y1, int x2, int y2);//选项选中