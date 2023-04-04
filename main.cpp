#include "head.h"
struct point //坐标属性
{
	int x;
	int y;
};
struct Snake //蛇的属性
{
	int num;       //蛇的节数
	point xy[100]; //蛇最长的节数
	char postion;  //方向
}snake;
struct Food //食物属性
{
	point foodxy; //食物坐标
	int flag;     //食物是否存在
}food;
struct User {
	int score = 0;
	char name[50];
}user;
// 定义变量，用于保存用户的选择
int music_on = 1,sound_on = 1;
enum moveposition { right = 77, left = 75, down = 80, up = 72 };
int state; 
bool isHit(ExMessage* m, int x1, int y1, int x2, int y2) {
	int x = m->x;
	int y = m->y;
	if (x > x1 && x<x2 && y>y1 && y < y2) {
		return true;
	}
	return false;
}
//判断蛇和食物是否相交
//设A[x01,y01,x02,y02]  B[x11,y11,x12,y12].左上角，右下角
bool rectIntersect(int x01, int y01, int x02, int y02,
	int x11, int y11, int x12, int y12)
{
	//求距离绝对值
	int zx = abs(x01 + x02 - x11 - x12);
	int x = abs(x01 - x02) + abs(x11 - x12);
	int zy = abs(y01 + y02 - y11 - y12);
	int y = abs(y01 - y02) + abs(y11 - y12);
	return  (zx <= x && zy <= y);
}
void intsnake()
{
	//初始化数据
	user.score = 0;//
	//初始位置
	snake.xy[2].x = 0;
	snake.xy[2].y = 0;
	snake.xy[1].x = 10;
	snake.xy[1].y = 0;
	snake.xy[0].x = 20;
	snake.xy[0].y = 0;
	//节数
	snake.num = 3;
	//初始方向
	snake.postion = down;
	//初始食物位置
	food.foodxy.x = rand() % 59 * 10;
	food.foodxy.y = rand() % 69 * 10;
}
//画蛇
void drawsnake()
{
	char score_[10] = { 0 };
	//显示分数
	_itoa(user.score, score_, 10);
	outtextxy(10, 10, "分数:");
	outtextxy(60, 10, score_);
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);//边框颜色
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));//填充颜色（随机色）
		//画矩形
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}
//控制蛇的移动
void movesnake()
{   //除了第一节（蛇头），后面每一节都是前一节的坐标
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}
	//蛇第一节的处理
	switch (snake.postion)
	{
	case right:
		snake.xy[0].x += 10;
		break;
	case left:
		snake.xy[0].x -= 10;
		break;
	case down:
		snake.xy[0].y += 10;
		break;
	case up:
		snake.xy[0].y -= 10;
		break;
	default:
		break;
	}
}
//按键控制
//例如往上走则不能读取下键
void keysnake()
{
	char userkey = 0;
	userkey = _getch();
	if (userkey == right && snake.postion != left)
	{
		snake.postion = right;
	}else  	if (userkey == down && snake.postion != up)
	{
		snake.postion = down;
	}else  	if (userkey == up && snake.postion != down)
	{
		snake.postion = up;
	}
	else  	if (userkey == left && snake.postion != right)
	{
		snake.postion = left;
	}
	
}
//随机生成食物
void intfood()
{
	//拆开写，防止蛇吃不到食物
	food.foodxy.x = rand() % 59 * 10;
	food.foodxy.y = rand() % 69 * 10;
	food.flag = 1;
	//如果食物出现在蛇身上的情况，则重新生成食物到不在蛇身体为止
	for (int i = 0; i > snake.num; i++)
	{

		if (food.foodxy.x == snake.xy[i].x && food.foodxy.y == snake.xy[i].y)
		{
			food.foodxy.x = rand() % 59 * 10;
			food.foodxy.y = rand() % 69 * 10;
		}
	}
}
//画食物
void drawfood()
{
	setlinecolor(BLACK);
	setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));//填充颜色（随机色）
	fillellipse(food.foodxy.x, food.foodxy.y, food.foodxy.x + 10, food.foodxy.y + 10);
}
//判断蛇是否吃食物
void eatfood()
{
	mciSendString("open music/eat.mp3 alias eat", 0, 0, 0);
	if (snake.xy[0].x == food.foodxy.x && snake.xy[0].y == food.foodxy.y)
	//if (rectIntersect(snake.xy[0].x, snake.xy[0].y, snake.xy[0].x +10, snake.xy[0].y + 10, food.foodxy.x, food.foodxy.y, food.foodxy.x + 10, food.foodxy.y + 10))
	{   //吃到食物的音效播放
		if (sound_on == 1)
		{
			mciSendString("play music/eat.mp3 ", 0, 0, 0);
		}
		user.score = user.score + 5;//分数加5
		snake.num++;//蛇长度每次加一
		food.flag = 0;//重新显示食物
	}
}
//判断蛇死亡的情况
int snakedie()
{
	if (snake.xy[0].x > 599 || snake.xy[0].y > 699 || snake.xy[0].x < 0 || snake.xy[0].y < 0)
	{
		if (sound_on == 1)
		{
			mciSendString("open music/kill.mp3", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
			mciSendString("play music/kill.mp3 ", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		}
		MessageBox(GetHWnd(), "撞墙死亡", "游戏结束", MB_OK);//弹出一个窗口
		ranking(user.score);//将得分进行排序
		return 1;
	}
	for (int i = 1; i < snake.num; i++)//撞自己
	{
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y)
		{
			if (sound_on == 1)
			{
				mciSendString("open music/kill.mp3", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
				mciSendString("play music/kill.mp3 ", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
			}
			MessageBox(GetHWnd(), "撞自己死亡", "游戏结束", MB_OK);//弹出一个窗口
			ranking(user.score);
			return 1;
		}
	}
	return 0;
}
//游戏界面
int gameview()
{
	//输入用户名保存记录
	InputBox(user.name, 50, "请输入你的用户名");
	//用户名限制
	for (int i = 0; i < 50; i++) {
		if (user.name[i] == NULL)
			user.name[i] = '\0';
		break;
	}
	//控制音乐
	if (music_on == 1)
	{
		mciSendString("close music/back.mp3 ", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		mciSendString("open music/gamemusic.mp3 ", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		mciSendString("play music/gamemusic.mp3 repeat", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		mciSendString("setaudio music/gamemusic.mp3 volume to 70", NULL, 0, NULL);
	}
	settextcolor(BLACK);//设置文字颜色
	settextstyle(20, 0, _T("楷体"));//设置文高度、平均宽度、字体
	setbkmode(TRANSPARENT);//设置文字背景为透明
	srand((unsigned int)time(NULL));//生成随机数种子
	intsnake();//初始化
	while (1)
	{
		//双缓冲绘图，放在绘图代码之前和之后，防止贪吃蛇界面一闪一闪
		BeginBatchDraw();//开始绘图
		//背景图片处理
		IMAGE BK;
		loadimage(&BK, "picture/BK.jpg", 0, 0);
		putimage(0, 0, &BK);

		drawsnake();//画蛇
		movesnake();//蛇移动
		drawfood();//画食物

		EndBatchDraw();// 结束绘图
		//随机生成食物
		if (food.flag == 0)
		{
			intfood();
		}
		//判断蛇是否吃到食物
		eatfood();
		//判断是否死亡触发显示排行榜
		if (snakedie())
		{
			showRank();
			break;
		}
		//监控键盘是否有输入
		while (_kbhit())
		{
			keysnake();
		}
		Sleep(50);//放慢速度
	}
	return 0;
}
//排行榜排序
void ranking(int score) {
	char fname[50] = { "ranking.txt" };//文件名
	FILE* fp;//声明文件指针
	errno_t err;

	int rank[8] = { 0 };
	char userName[8][50];//上限记录8名用户
	int flag = 0;//是否新用户，新用户则为0，旧用户则为1
	char n = '\n';

	//读取前八名用户和分数
	if ((err = fopen_s(&fp, fname, "r")) != 0) {
		printf("不能打开文件%s.\n", fname);
		return;
	}
	for (int i = 0; i < 8; i++) {
		fgets(userName[i], 50, fp);
		fscanf_s(fp, "%d\n", &rank[i]);
		printf("从文件输入：%s %d\n", userName[i], rank[i]);
	}
	fclose(fp);

	printf("用户名为%s\n", user.name);

	//若为旧用户且在排行榜上，需更新分数并重新排序
	printf("userName[0]=%s,user.name=%s\n", userName[0], user.name);
	for (int i = 0; i < 8; i++) {
		int num = 0;
		printf("字符串长度为%d\n", strlen(userName[i]) - 1);
		for (int j = 0; j < strlen(userName[i]) - 1; j++) {
			if (userName[i][j] == user.name[j]) num++;
		}
		printf("num=%d\n", num);
		if (num == strlen(userName[i]) - 1) {
			flag = 1;
			printf("为旧用户\n");
			if (rank[i] <= user.score) {
				rank[i] = user.score;
			}
			//排序
			sort(rank, userName);
			break;
		}
	}

	printf("flag=%d\n", flag);
	//新用户排序
	if (flag == 0) {
		printf("为新用户\n");
		for (int i = 0; i < 8; i++) {
			if (user.score >= rank[i]) {
				for (int j = 7; j > i; j--) {
					rank[j] = rank[j - 1];
					strcpy(userName[j], userName[j - 1]);
				}
				rank[i] = user.score;
				strcpy(userName[i], user.name);
				break;
			}
			//fscanf_s(fp, "%d", rank[i]);
		}

	}

	//printf("原分数为%d", pre);
	fclose(fp);



	//判断是否在前八，如果在前八则修改分数
	//将新排行榜输入文件
	if ((err = fopen_s(&fp, fname, "w")) != 0) {
		printf("不能打开文件%s.\n", fname);
		return;
	}
	for (int i = 0; i < 8; i++) {
		fputs(userName[i], fp);
		if (strcmp(userName[i], user.name) == 0) {
			fprintf(fp, "\n");
		}
		fprintf(fp, "%d\n", rank[i]);
		printf("输入文件：%s %d\n", userName[i], rank[i]);
	}

	fclose(fp);

}

//分数从高到低排序
void sort(int rank[], char userName[][50]) 
{
	int temp = 0;
	char tmp[50];
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7 - i; j++) {
			if (rank[j] < rank[j + 1]) {
				temp = rank[j + 1];
				rank[j + 1] = rank[j];
				rank[j] = temp;
				strcpy(tmp, userName[j]);
				strcpy(userName[j + 1], userName[j]);
				strcpy(userName[j], tmp);
			}
		}
	}
}

//显示排行榜界面
int showRank() {
	//控制音乐
	if (sound_on == 1)
	{
		mciSendString("close music/eat.mp3 ", 0, 0, 0);
		mciSendString("close music/kill.mp3 ", 0, 0, 0);
		mciSendString("close music/gamemusic.mp3 ", 0, 0, 0);
		mciSendString("open music/ranking.mp3", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		mciSendString("play music/ranking.mp3 ", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		mciSendString("setaudiomusic/ranking.mp3  volume to 20", NULL, 0, NULL);
	}
	cleardevice();//页面切换清屏
	BeginBatchDraw();//开始绘图
	//背景图片处理
	IMAGE BK;
	loadimage(&BK, "picture/BK.jpg", 0, 0);
	putimage(0, 0, &BK);
	EndBatchDraw();// 结束绘图

	//从txt文件载入排名
	char fname[50] = { "ranking.txt" };//文件名
	FILE* fp;//声明文件指针
	errno_t err;
	char hScore[50];
	int rank[8] = { 0 };
	char userName[8][50];//设定上限8名用户

	//读取前八名用户
	if ((err = fopen_s(&fp, fname, "r")) != 0) {
		printf("不能打开文件%s.\n", fname);
		return 4;
	}
	for (int i = 0; i < 8; i++) 
	{
		fgets(userName[i], 50, fp);
		fscanf_s(fp, "%d\n", &rank[i]);
		printf("显示时从文件输入：%s %d\n", userName[i], rank[i]);
	}
	fclose(fp);

	setbkmode(TRANSPARENT);//设置文字背景为透明
	settextcolor(BLACK);//设置文字颜色
	settextstyle(20, 0, _T("华文新魏"));//设置文高度、平均宽度、字体

	//显示用户名，分数，排名
	for (int i = 0; i < 8; i++) {
		outtextxy(20, 100 + 50 * i, userName[i]);
		sprintf(hScore, "%d", rank[i]);
		outtextxy(320, 100 + 50 * i, hScore);

	}
	settextstyle(50, 0, _T("华文新魏"));
	outtextxy(225, 20, "排行榜");
	settextstyle(20, 0, _T("华文新魏"));
	outtextxy(520, 100, "第一名");
	outtextxy(520, 100 + 50 * 1, "第二名");
	outtextxy(520, 100 + 50 * 2, "第三名");
	outtextxy(520, 100 + 50 * 3, "第四名");
	outtextxy(520, 100 + 50 * 4, "第五名");
	outtextxy(520, 100 + 50 * 5, "第六名");
	outtextxy(520, 100 + 50 * 6, "第七名");
	outtextxy(520, 100 + 50 * 7, "第八名");
	
	//显示返回主菜单
	int width = 20 / 2 - textwidth("返回") / 2;
	int height = 15 / 2 - textheight("返回") / 2;
	outtextxy(width + 20, height + 15, "返回");

	//监控鼠标是否点击返回主菜单
	while (1)
	{
		// 监听鼠标事件
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			// 点击返回按钮
			if (msg.x > 0 && msg.x < 40 && msg.y > 0 && msg.y < 40)
			{ 
				//控制音乐
				mciSendString("close music/ranking.mp3 ", 0, 0, 0);
				mciSendString("close music/gamemusic.mp3 ", 0, 0, 0);
				break;
			}
		}
	}
	//跳转主菜单
	state = 1;
	//控制音乐
	if (music_on == 1)
	{
		mciSendString("open music/back.mp3 ", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		mciSendString("play music/back.mp3 repeat", 0, 0, 0);//打开文件并给mp3取别名（alias）为a
		mciSendString("setaudio music/back.mp3  volume to 50", NULL, 0, NULL);
	}
	return 0;
}

int menu()
{
	BeginBatchDraw();
	//声明对象
	IMAGE Title, Start, Setting, Help, Exit,View;
	//加载图片
	loadimage(&View,"picture/View.jpg", 600, 700);
	loadimage(&Start, ("picture/Start.jpg"), 160, 60);
	loadimage(&Setting, ("picture/SET.png"), 160, 60);
	loadimage(&Help, ("picture/HELP.jpg"), 160, 60);
	loadimage(&Exit, ("picture/END.png"), 160, 60);
	//放置图片
	putimage(0, 0, &View);
	putimage(60, 190, &Start);
	putimage(380, 190, &Setting);
	putimage(370, 360, &Help);
	putimage(60, 360, &Exit);
	//鼠标监控主菜单选项
	ExMessage  m;
	m = getmessage();
	switch (m.message)
	{
	case WM_LBUTTONDOWN:
	{
		if (isHit(&m, 60, 190, 226, 225))//开始游戏
		{
			state = 2;
		}
		else if (isHit(&m, 380, 190, 541, 252))//设置
		{
			state = 3;
		}
		else if (isHit(&m, 370, 360, 532, 423))//帮助
		{
			state = 4;
		}
		else if (isHit(&m, 60, 360, 220, 424))//退出游戏
		{
			state = 5;
		}
	}
		}
	EndBatchDraw();
	return 0;
}
int Asist()
{
	//查看游戏说明
	//返回主菜单
	setbkcolor(RGB(164, 225, 202));
	cleardevice();
	//声明IMAGE对象
	IMAGE img;
	//加载背景图
	loadimage(&img, "picture/bkround.jpg",610,700);
	putimage(0, 0, &img);


	RECT rect;
	rect.left = 0;
	rect.top = 50;
	rect.right = 640;
	rect.bottom = 450;
	rectangle(rect.left, rect.top, rect.right, rect.bottom);
	settextcolor(RGB(255, 128, 0));
	settextstyle(30, 0, "华文新魏");
	setbkmode(TRANSPARENT);
	const char* pStr =
	{
	  "游戏说明\n ●小蛇想要长大，希望你能帮帮他\n"
	  "●通过↑, ↓, ←, →操纵小蛇吃到食物，需要玩家用你们灵活的手指控制小蛇，不要撞到自己的身体和围墙，帮助小蛇完成梦想，祝你们好运！\n"
	  "●制作组成员：李深，焖鱼，飞行员，小言，Keriyar，热牛奶，言欣"
	};

	const char* pText = "返回";
	outtextxy(0, 0, pText);
	drawtext(pStr, &rect, DT_CENTER | DT_WORDBREAK);
	// 监听鼠标事件
	while (true)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			// 点击返回按钮
			if (msg.x > 0 && msg.x < 40 && msg.y > 0 && msg.y < 40)
			{
				break;
			}
		}
	}
	//跳转主菜单
	state = 1;
	return 0;
}
//控制音乐、音效开关
void setView()
{
	//返回主菜单
	cleardevice();
	//声明图片对象
	IMAGE image_setback, image_musicoff, image_musicon, image_soundon,image_home,image_soundoff;
	// 加载图片
	loadimage(&image_setback, "picture/setback.png",610,710);
	loadimage(&image_home,"picture/Return.jpg",100,50);
	loadimage(&image_musicoff, "picture/musicoff.png");
	loadimage(&image_musicon, "picture/musicon.png");
	loadimage(&image_soundon, "picture/soundon.png");
	loadimage(&image_soundoff, "picture/soundoff.png");
	
	//放置图片
	//背景
	putimage(0, 0, &image_setback);
	// 返回首页图片
	putimage(1, 1, &image_home);
	if (music_on == 1)
	{
		//放置音乐图片
		putimage(150, 300, &image_musicon);
	}else
	{
		//放置音乐图片
		putimage(150, 300, &image_musicoff);
	}
	if (sound_on == 1)
	{
		//放置音效图片
		putimage(150, 500, &image_soundon);
	}
	else 
	{
		//放置音效图片
		putimage(150,500, &image_soundoff);
	}
	// 循环等待用户点击按钮
	while (1)
	{
		// 监听鼠标事件
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			// 点击音乐开关按钮
			if (msg.x > 360 && msg.x < 470 && msg.y > 330 && msg.y < 365)
			{
				printf("music_on=%d\r\n", music_on);//测试
				if (music_on == 0)
				{
					mciSendString("open music/back.mp3 alias g", 0, 0, 0);
					mciSendString("play music/back.mp3 repeat", 0, 0, 0);
					mciSendString("setaudio music/back.mp3  volume to 50", NULL, 0, NULL);
					putimage(150, 300, &image_musicon);
					music_on = 1;//打开
				}
				else if (music_on == 1)
				{
					mciSendString("close music/back.mp3 ", 0, 0, 0);
					putimage(150, 300, &image_musicoff);
					music_on = 0;//关闭
				}
			}
			// 点击音效开关按钮
			if (msg.x > 360 && msg.x < 470 && msg.y > 530 && msg.y < 565)
			{
				printf("sound_on=%d\r\n", sound_on);//测试
				if (sound_on == 0)
				{
					mciSendString("open music/kill.mp3", 0, 0, 0);
					mciSendString("open music/eat.mp3", 0, 0, 0);
					mciSendString("open music/ramking.mp3", 0, 0, 0);
					mciSendString("setaudio  music/ramking.mp3 volume to 50", NULL, 0, NULL);
					putimage(150, 500, &image_soundon);
					sound_on = 1;//打开
				}
				else if (sound_on == 1)
				{
					mciSendString("close music/eat.mp3", 0, 0, 0);
					mciSendString("close music/kill.mp3", 0, 0, 0);
					mciSendString("close music/ramking.mp3", 0, 0, 0);
					putimage(150, 500, &image_soundoff);
					sound_on = 0;//关闭
				}
			}
			//监控左上角的返回
			if (msg.x > 0 && msg.x < 100 && msg.y > 0 && msg.y < 50)
			{
				MessageBox(GetHWnd(), "设置成功，正在返回主菜单", "提醒!", MB_OK);//弹窗提醒
				state = 1;
				break;
			}
		}
	}
}
int main()
{
	state = 1;
	initgraph(600, 700);//窗口大小
	//加载音乐
	mciSendString("open music/back.mp3 alias g", 0, 0, 0);
	mciSendString("play music/back.mp3 repeat", 0, 0, 0);
	mciSendString("setaudio music/back.mp3  volume to 50", NULL, 0, NULL);
	while (1) {
		switch (state)
		{
		case 1:
			//调用主菜单界面
			menu();
			break;
		case 2:
			//调用开始游戏界面
			gameview();
			break;

		case 3:
			//调用设置界面	
			setView();
			break;
		case 4:
			//帮助界面
			Asist();
			break;
		case 5:
			//退出游戏
			exit(0);
			break;
		}
	}
	getchar();//防止闪屏
	closegraph();//关闭窗口
	return 0;
}