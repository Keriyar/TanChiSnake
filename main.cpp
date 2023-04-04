#include "head.h"
struct point //��������
{
	int x;
	int y;
};
struct Snake //�ߵ�����
{
	int num;       //�ߵĽ���
	point xy[100]; //����Ľ���
	char postion;  //����
}snake;
struct Food //ʳ������
{
	point foodxy; //ʳ������
	int flag;     //ʳ���Ƿ����
}food;
struct User {
	int score = 0;
	char name[50];
}user;
// ������������ڱ����û���ѡ��
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
//�ж��ߺ�ʳ���Ƿ��ཻ
//��A[x01,y01,x02,y02]  B[x11,y11,x12,y12].���Ͻǣ����½�
bool rectIntersect(int x01, int y01, int x02, int y02,
	int x11, int y11, int x12, int y12)
{
	//��������ֵ
	int zx = abs(x01 + x02 - x11 - x12);
	int x = abs(x01 - x02) + abs(x11 - x12);
	int zy = abs(y01 + y02 - y11 - y12);
	int y = abs(y01 - y02) + abs(y11 - y12);
	return  (zx <= x && zy <= y);
}
void intsnake()
{
	//��ʼ������
	user.score = 0;//
	//��ʼλ��
	snake.xy[2].x = 0;
	snake.xy[2].y = 0;
	snake.xy[1].x = 10;
	snake.xy[1].y = 0;
	snake.xy[0].x = 20;
	snake.xy[0].y = 0;
	//����
	snake.num = 3;
	//��ʼ����
	snake.postion = down;
	//��ʼʳ��λ��
	food.foodxy.x = rand() % 59 * 10;
	food.foodxy.y = rand() % 69 * 10;
}
//����
void drawsnake()
{
	char score_[10] = { 0 };
	//��ʾ����
	_itoa(user.score, score_, 10);
	outtextxy(10, 10, "����:");
	outtextxy(60, 10, score_);
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);//�߿���ɫ
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));//�����ɫ�����ɫ��
		//������
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}
//�����ߵ��ƶ�
void movesnake()
{   //���˵�һ�ڣ���ͷ��������ÿһ�ڶ���ǰһ�ڵ�����
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}
	//�ߵ�һ�ڵĴ���
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
//��������
//�������������ܶ�ȡ�¼�
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
//�������ʳ��
void intfood()
{
	//��д����ֹ�߳Բ���ʳ��
	food.foodxy.x = rand() % 59 * 10;
	food.foodxy.y = rand() % 69 * 10;
	food.flag = 1;
	//���ʳ������������ϵ����������������ʳ�ﵽ����������Ϊֹ
	for (int i = 0; i > snake.num; i++)
	{

		if (food.foodxy.x == snake.xy[i].x && food.foodxy.y == snake.xy[i].y)
		{
			food.foodxy.x = rand() % 59 * 10;
			food.foodxy.y = rand() % 69 * 10;
		}
	}
}
//��ʳ��
void drawfood()
{
	setlinecolor(BLACK);
	setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));//�����ɫ�����ɫ��
	fillellipse(food.foodxy.x, food.foodxy.y, food.foodxy.x + 10, food.foodxy.y + 10);
}
//�ж����Ƿ��ʳ��
void eatfood()
{
	mciSendString("open music/eat.mp3 alias eat", 0, 0, 0);
	if (snake.xy[0].x == food.foodxy.x && snake.xy[0].y == food.foodxy.y)
	//if (rectIntersect(snake.xy[0].x, snake.xy[0].y, snake.xy[0].x +10, snake.xy[0].y + 10, food.foodxy.x, food.foodxy.y, food.foodxy.x + 10, food.foodxy.y + 10))
	{   //�Ե�ʳ�����Ч����
		if (sound_on == 1)
		{
			mciSendString("play music/eat.mp3 ", 0, 0, 0);
		}
		user.score = user.score + 5;//������5
		snake.num++;//�߳���ÿ�μ�һ
		food.flag = 0;//������ʾʳ��
	}
}
//�ж������������
int snakedie()
{
	if (snake.xy[0].x > 599 || snake.xy[0].y > 699 || snake.xy[0].x < 0 || snake.xy[0].y < 0)
	{
		if (sound_on == 1)
		{
			mciSendString("open music/kill.mp3", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
			mciSendString("play music/kill.mp3 ", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		}
		MessageBox(GetHWnd(), "ײǽ����", "��Ϸ����", MB_OK);//����һ������
		ranking(user.score);//���÷ֽ�������
		return 1;
	}
	for (int i = 1; i < snake.num; i++)//ײ�Լ�
	{
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y)
		{
			if (sound_on == 1)
			{
				mciSendString("open music/kill.mp3", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
				mciSendString("play music/kill.mp3 ", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
			}
			MessageBox(GetHWnd(), "ײ�Լ�����", "��Ϸ����", MB_OK);//����һ������
			ranking(user.score);
			return 1;
		}
	}
	return 0;
}
//��Ϸ����
int gameview()
{
	//�����û��������¼
	InputBox(user.name, 50, "����������û���");
	//�û�������
	for (int i = 0; i < 50; i++) {
		if (user.name[i] == NULL)
			user.name[i] = '\0';
		break;
	}
	//��������
	if (music_on == 1)
	{
		mciSendString("close music/back.mp3 ", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		mciSendString("open music/gamemusic.mp3 ", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		mciSendString("play music/gamemusic.mp3 repeat", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		mciSendString("setaudio music/gamemusic.mp3 volume to 70", NULL, 0, NULL);
	}
	settextcolor(BLACK);//����������ɫ
	settextstyle(20, 0, _T("����"));//�����ĸ߶ȡ�ƽ����ȡ�����
	setbkmode(TRANSPARENT);//�������ֱ���Ϊ͸��
	srand((unsigned int)time(NULL));//�������������
	intsnake();//��ʼ��
	while (1)
	{
		//˫�����ͼ�����ڻ�ͼ����֮ǰ��֮�󣬷�ֹ̰���߽���һ��һ��
		BeginBatchDraw();//��ʼ��ͼ
		//����ͼƬ����
		IMAGE BK;
		loadimage(&BK, "picture/BK.jpg", 0, 0);
		putimage(0, 0, &BK);

		drawsnake();//����
		movesnake();//���ƶ�
		drawfood();//��ʳ��

		EndBatchDraw();// ������ͼ
		//�������ʳ��
		if (food.flag == 0)
		{
			intfood();
		}
		//�ж����Ƿ�Ե�ʳ��
		eatfood();
		//�ж��Ƿ�����������ʾ���а�
		if (snakedie())
		{
			showRank();
			break;
		}
		//��ؼ����Ƿ�������
		while (_kbhit())
		{
			keysnake();
		}
		Sleep(50);//�����ٶ�
	}
	return 0;
}
//���а�����
void ranking(int score) {
	char fname[50] = { "ranking.txt" };//�ļ���
	FILE* fp;//�����ļ�ָ��
	errno_t err;

	int rank[8] = { 0 };
	char userName[8][50];//���޼�¼8���û�
	int flag = 0;//�Ƿ����û������û���Ϊ0�����û���Ϊ1
	char n = '\n';

	//��ȡǰ�����û��ͷ���
	if ((err = fopen_s(&fp, fname, "r")) != 0) {
		printf("���ܴ��ļ�%s.\n", fname);
		return;
	}
	for (int i = 0; i < 8; i++) {
		fgets(userName[i], 50, fp);
		fscanf_s(fp, "%d\n", &rank[i]);
		printf("���ļ����룺%s %d\n", userName[i], rank[i]);
	}
	fclose(fp);

	printf("�û���Ϊ%s\n", user.name);

	//��Ϊ���û��������а��ϣ�����·�������������
	printf("userName[0]=%s,user.name=%s\n", userName[0], user.name);
	for (int i = 0; i < 8; i++) {
		int num = 0;
		printf("�ַ�������Ϊ%d\n", strlen(userName[i]) - 1);
		for (int j = 0; j < strlen(userName[i]) - 1; j++) {
			if (userName[i][j] == user.name[j]) num++;
		}
		printf("num=%d\n", num);
		if (num == strlen(userName[i]) - 1) {
			flag = 1;
			printf("Ϊ���û�\n");
			if (rank[i] <= user.score) {
				rank[i] = user.score;
			}
			//����
			sort(rank, userName);
			break;
		}
	}

	printf("flag=%d\n", flag);
	//���û�����
	if (flag == 0) {
		printf("Ϊ���û�\n");
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

	//printf("ԭ����Ϊ%d", pre);
	fclose(fp);



	//�ж��Ƿ���ǰ�ˣ������ǰ�����޸ķ���
	//�������а������ļ�
	if ((err = fopen_s(&fp, fname, "w")) != 0) {
		printf("���ܴ��ļ�%s.\n", fname);
		return;
	}
	for (int i = 0; i < 8; i++) {
		fputs(userName[i], fp);
		if (strcmp(userName[i], user.name) == 0) {
			fprintf(fp, "\n");
		}
		fprintf(fp, "%d\n", rank[i]);
		printf("�����ļ���%s %d\n", userName[i], rank[i]);
	}

	fclose(fp);

}

//�����Ӹߵ�������
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

//��ʾ���а����
int showRank() {
	//��������
	if (sound_on == 1)
	{
		mciSendString("close music/eat.mp3 ", 0, 0, 0);
		mciSendString("close music/kill.mp3 ", 0, 0, 0);
		mciSendString("close music/gamemusic.mp3 ", 0, 0, 0);
		mciSendString("open music/ranking.mp3", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		mciSendString("play music/ranking.mp3 ", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		mciSendString("setaudiomusic/ranking.mp3  volume to 20", NULL, 0, NULL);
	}
	cleardevice();//ҳ���л�����
	BeginBatchDraw();//��ʼ��ͼ
	//����ͼƬ����
	IMAGE BK;
	loadimage(&BK, "picture/BK.jpg", 0, 0);
	putimage(0, 0, &BK);
	EndBatchDraw();// ������ͼ

	//��txt�ļ���������
	char fname[50] = { "ranking.txt" };//�ļ���
	FILE* fp;//�����ļ�ָ��
	errno_t err;
	char hScore[50];
	int rank[8] = { 0 };
	char userName[8][50];//�趨����8���û�

	//��ȡǰ�����û�
	if ((err = fopen_s(&fp, fname, "r")) != 0) {
		printf("���ܴ��ļ�%s.\n", fname);
		return 4;
	}
	for (int i = 0; i < 8; i++) 
	{
		fgets(userName[i], 50, fp);
		fscanf_s(fp, "%d\n", &rank[i]);
		printf("��ʾʱ���ļ����룺%s %d\n", userName[i], rank[i]);
	}
	fclose(fp);

	setbkmode(TRANSPARENT);//�������ֱ���Ϊ͸��
	settextcolor(BLACK);//����������ɫ
	settextstyle(20, 0, _T("������κ"));//�����ĸ߶ȡ�ƽ����ȡ�����

	//��ʾ�û���������������
	for (int i = 0; i < 8; i++) {
		outtextxy(20, 100 + 50 * i, userName[i]);
		sprintf(hScore, "%d", rank[i]);
		outtextxy(320, 100 + 50 * i, hScore);

	}
	settextstyle(50, 0, _T("������κ"));
	outtextxy(225, 20, "���а�");
	settextstyle(20, 0, _T("������κ"));
	outtextxy(520, 100, "��һ��");
	outtextxy(520, 100 + 50 * 1, "�ڶ���");
	outtextxy(520, 100 + 50 * 2, "������");
	outtextxy(520, 100 + 50 * 3, "������");
	outtextxy(520, 100 + 50 * 4, "������");
	outtextxy(520, 100 + 50 * 5, "������");
	outtextxy(520, 100 + 50 * 6, "������");
	outtextxy(520, 100 + 50 * 7, "�ڰ���");
	
	//��ʾ�������˵�
	int width = 20 / 2 - textwidth("����") / 2;
	int height = 15 / 2 - textheight("����") / 2;
	outtextxy(width + 20, height + 15, "����");

	//�������Ƿ����������˵�
	while (1)
	{
		// ��������¼�
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			// ������ذ�ť
			if (msg.x > 0 && msg.x < 40 && msg.y > 0 && msg.y < 40)
			{ 
				//��������
				mciSendString("close music/ranking.mp3 ", 0, 0, 0);
				mciSendString("close music/gamemusic.mp3 ", 0, 0, 0);
				break;
			}
		}
	}
	//��ת���˵�
	state = 1;
	//��������
	if (music_on == 1)
	{
		mciSendString("open music/back.mp3 ", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		mciSendString("play music/back.mp3 repeat", 0, 0, 0);//���ļ�����mp3ȡ������alias��Ϊa
		mciSendString("setaudio music/back.mp3  volume to 50", NULL, 0, NULL);
	}
	return 0;
}

int menu()
{
	BeginBatchDraw();
	//��������
	IMAGE Title, Start, Setting, Help, Exit,View;
	//����ͼƬ
	loadimage(&View,"picture/View.jpg", 600, 700);
	loadimage(&Start, ("picture/Start.jpg"), 160, 60);
	loadimage(&Setting, ("picture/SET.png"), 160, 60);
	loadimage(&Help, ("picture/HELP.jpg"), 160, 60);
	loadimage(&Exit, ("picture/END.png"), 160, 60);
	//����ͼƬ
	putimage(0, 0, &View);
	putimage(60, 190, &Start);
	putimage(380, 190, &Setting);
	putimage(370, 360, &Help);
	putimage(60, 360, &Exit);
	//��������˵�ѡ��
	ExMessage  m;
	m = getmessage();
	switch (m.message)
	{
	case WM_LBUTTONDOWN:
	{
		if (isHit(&m, 60, 190, 226, 225))//��ʼ��Ϸ
		{
			state = 2;
		}
		else if (isHit(&m, 380, 190, 541, 252))//����
		{
			state = 3;
		}
		else if (isHit(&m, 370, 360, 532, 423))//����
		{
			state = 4;
		}
		else if (isHit(&m, 60, 360, 220, 424))//�˳���Ϸ
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
	//�鿴��Ϸ˵��
	//�������˵�
	setbkcolor(RGB(164, 225, 202));
	cleardevice();
	//����IMAGE����
	IMAGE img;
	//���ر���ͼ
	loadimage(&img, "picture/bkround.jpg",610,700);
	putimage(0, 0, &img);


	RECT rect;
	rect.left = 0;
	rect.top = 50;
	rect.right = 640;
	rect.bottom = 450;
	rectangle(rect.left, rect.top, rect.right, rect.bottom);
	settextcolor(RGB(255, 128, 0));
	settextstyle(30, 0, "������κ");
	setbkmode(TRANSPARENT);
	const char* pStr =
	{
	  "��Ϸ˵��\n ��С����Ҫ����ϣ�����ܰ����\n"
	  "��ͨ����, ��, ��, ������С�߳Ե�ʳ���Ҫ���������������ָ����С�ߣ���Ҫײ���Լ��������Χǽ������С��������룬ף���Ǻ��ˣ�\n"
	  "���������Ա��������㣬����Ա��С�ԣ�Keriyar����ţ�̣�����"
	};

	const char* pText = "����";
	outtextxy(0, 0, pText);
	drawtext(pStr, &rect, DT_CENTER | DT_WORDBREAK);
	// ��������¼�
	while (true)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			// ������ذ�ť
			if (msg.x > 0 && msg.x < 40 && msg.y > 0 && msg.y < 40)
			{
				break;
			}
		}
	}
	//��ת���˵�
	state = 1;
	return 0;
}
//�������֡���Ч����
void setView()
{
	//�������˵�
	cleardevice();
	//����ͼƬ����
	IMAGE image_setback, image_musicoff, image_musicon, image_soundon,image_home,image_soundoff;
	// ����ͼƬ
	loadimage(&image_setback, "picture/setback.png",610,710);
	loadimage(&image_home,"picture/Return.jpg",100,50);
	loadimage(&image_musicoff, "picture/musicoff.png");
	loadimage(&image_musicon, "picture/musicon.png");
	loadimage(&image_soundon, "picture/soundon.png");
	loadimage(&image_soundoff, "picture/soundoff.png");
	
	//����ͼƬ
	//����
	putimage(0, 0, &image_setback);
	// ������ҳͼƬ
	putimage(1, 1, &image_home);
	if (music_on == 1)
	{
		//��������ͼƬ
		putimage(150, 300, &image_musicon);
	}else
	{
		//��������ͼƬ
		putimage(150, 300, &image_musicoff);
	}
	if (sound_on == 1)
	{
		//������ЧͼƬ
		putimage(150, 500, &image_soundon);
	}
	else 
	{
		//������ЧͼƬ
		putimage(150,500, &image_soundoff);
	}
	// ѭ���ȴ��û������ť
	while (1)
	{
		// ��������¼�
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			// ������ֿ��ذ�ť
			if (msg.x > 360 && msg.x < 470 && msg.y > 330 && msg.y < 365)
			{
				printf("music_on=%d\r\n", music_on);//����
				if (music_on == 0)
				{
					mciSendString("open music/back.mp3 alias g", 0, 0, 0);
					mciSendString("play music/back.mp3 repeat", 0, 0, 0);
					mciSendString("setaudio music/back.mp3  volume to 50", NULL, 0, NULL);
					putimage(150, 300, &image_musicon);
					music_on = 1;//��
				}
				else if (music_on == 1)
				{
					mciSendString("close music/back.mp3 ", 0, 0, 0);
					putimage(150, 300, &image_musicoff);
					music_on = 0;//�ر�
				}
			}
			// �����Ч���ذ�ť
			if (msg.x > 360 && msg.x < 470 && msg.y > 530 && msg.y < 565)
			{
				printf("sound_on=%d\r\n", sound_on);//����
				if (sound_on == 0)
				{
					mciSendString("open music/kill.mp3", 0, 0, 0);
					mciSendString("open music/eat.mp3", 0, 0, 0);
					mciSendString("open music/ramking.mp3", 0, 0, 0);
					mciSendString("setaudio  music/ramking.mp3 volume to 50", NULL, 0, NULL);
					putimage(150, 500, &image_soundon);
					sound_on = 1;//��
				}
				else if (sound_on == 1)
				{
					mciSendString("close music/eat.mp3", 0, 0, 0);
					mciSendString("close music/kill.mp3", 0, 0, 0);
					mciSendString("close music/ramking.mp3", 0, 0, 0);
					putimage(150, 500, &image_soundoff);
					sound_on = 0;//�ر�
				}
			}
			//������Ͻǵķ���
			if (msg.x > 0 && msg.x < 100 && msg.y > 0 && msg.y < 50)
			{
				MessageBox(GetHWnd(), "���óɹ������ڷ������˵�", "����!", MB_OK);//��������
				state = 1;
				break;
			}
		}
	}
}
int main()
{
	state = 1;
	initgraph(600, 700);//���ڴ�С
	//��������
	mciSendString("open music/back.mp3 alias g", 0, 0, 0);
	mciSendString("play music/back.mp3 repeat", 0, 0, 0);
	mciSendString("setaudio music/back.mp3  volume to 50", NULL, 0, NULL);
	while (1) {
		switch (state)
		{
		case 1:
			//�������˵�����
			menu();
			break;
		case 2:
			//���ÿ�ʼ��Ϸ����
			gameview();
			break;

		case 3:
			//�������ý���	
			setView();
			break;
		case 4:
			//��������
			Asist();
			break;
		case 5:
			//�˳���Ϸ
			exit(0);
			break;
		}
	}
	getchar();//��ֹ����
	closegraph();//�رմ���
	return 0;
}