#include "mainmenurender.h"

DWORD globalStartTime;
char globalBuffer[1024];

void RearrangeString(char* res, const char* STR, int eTime, int maxLen) // maxLen <= 32
{
	int len = strlen(STR);
	if (len < maxLen)
	{
		strcpy(res, STR);
		return;
	}

	int spaceLen = 5;
	int delta = (eTime / TIME_INTERVAL) % (len + spaceLen);

	strcpy(res, STR + (delta > len - 1 ? len : delta));
	for (int i = 0; i < ((len + spaceLen - delta) > spaceLen ? spaceLen : (len + spaceLen - delta)); ++i)
	{
		strcat(res, " ");
	}
	strcat(res, STR);

	res[maxLen] = '\0';
}

void OutPutString(int x1, int y1, int x2, int y2, const char* arr)								//居中输出字符串
{
	int width = textwidth(arr[0]);
	int height = textheight(arr[0]);
	settextstyle(20, 0, _T("Bodoni MT"));
	outtextxy(x1 + (x2 - x1) / 2 - width / 2, y1 + (y2 - y1) / 2 - height / 2, arr);
}
void InitSelectCatalogue(int Place)								//选择界面 固定每个选项窗口大小为244*60 起始x坐标为320 共五个窗口
{
	int deltaTime = (timeGetTime() - globalStartTime) / TIME_INTERVAL;

	int x1 = 320, x2 = 564, y1 = 18, y2 = 78;
	int Yinc = 96;
	const int oX = 10, oX1 = deltaTime % 2 ? -5 : 0;
	const int oY = 6, oY1 = deltaTime % 2 ? -3 : 0;
	const int r = 10;
	for (int i = 0; i < 5; i++)
	{
		setfillcolor(0x703426);
		if (i + 1 == Place)
			solidroundrect(x1 + oX + oX1, y1 + oY + oY1, x2 + oX + oX1, y2 + oY + oY1, r, r);
		else
			solidroundrect(x1 + oX, y1 + oY, x2 + oX, y2 + oY, r, r);
		setfillcolor(0xFC7456);
		solidroundrect(x1, y1, x2, y2, r, r);
		y1 += Yinc, y2 += Yinc;
	}
}
void ShowCatalogueBox(BMSList B, int Place)								//单个选项内容
{
	int deltaTime = timeGetTime() - globalStartTime;

	setcolor(WHITE);
	setbkmode(TRANSPARENT);
	Place -= 1;
	int x1 = 330, x2 = 564, y1, y2;
	y1 = Place * 96 + 20;
	y2 = y1 + 20;
	settextstyle(20, 0, _T("Monaco"));

	RearrangeString(globalBuffer, B->data->title, deltaTime, 32);
	outtextxy(x1, y1, globalBuffer);
	y1 += 20;
	y2 = y1 + 20;
	RearrangeString(globalBuffer, B->data->artist, deltaTime, 32);
	outtextxy(x1, y1, globalBuffer);
	y1 += 20;
	y2 = y1 + 20;
	outtextxy(x1, y1, B->data->id);
}
void ShowInfoSheet(BMSList B, int Place)							//信息显示
{
	BMSList temp = B;
	int nowplace = Place;										//信息要显示的位置
	while (nowplace <= 5 && temp)									//从初始位置到第五格依次显示
	{
		ShowCatalogueBox(temp, nowplace);
		temp = temp->next;
		nowplace++;
	}
	nowplace = Place - 1;
	temp = B->prev;
	while (nowplace > 0 && temp)										//从初始位置上一位到第一格依次显示
	{
		ShowCatalogueBox(temp, nowplace);
		temp = temp->prev;
		nowplace--;
	}
}
void JudgeTriangleP(int* Place)
{
	if (*Place < 1)
		*Place = 1;
	if (*Place >= 6)
		*Place = 5;
}
void ShowTriangle(int Place)										//三角选择箭头显示
{
	const int oX = 2;
	const int oY = 2;
	setfillcolor(0x703426);
	Place -= 1;
	POINT points[3];
	points[0].x = 310;
	points[1].x = 290;
	points[2].x = 290;
	points[0].y = Place * 96 + 48;
	points[1].y = Place * 96 + 30;
	points[2].y = Place * 96 + 66;
	solidpolygon(points, 3);
	setfillcolor(0xD66349);
	points[0].x -= oX;
	points[1].x -= oX;
	points[2].x -= oX;
	points[0].y -= oY;
	points[1].y -= oY;
	points[2].y -= oY;
	solidpolygon(points, 3);
}
void ShowDif(BMSList B, int Place, int dif)									//难度选择状态显示
{
	setcolor(WHITE);
	setfillcolor(0x703426);
	//setcolor(0x703426);
	//setfillcolor(0xB0513C);
	Place -= 1;
	int x = 550, y;
	y = Place * 96 + 68;
	int Xinc = 15;
	for (int i = B->data->osuFileCnt - 1; i >= 0; --i)
	{
		if (i == dif)
			solidcircle(x, y, 5);
		circle(x, y, 5);
		x -= Xinc;
	}
}
void JudgeDif(int* dif, BMSList B)
{
	if (*dif < 0)
		*dif = B->data->osuFileCnt - 1;
	if (*dif >= B->data->osuFileCnt)
		*dif = 0;
}

void DrawTextPreview(BMSList bmsl, int dif, int sor, int sx, int sy, int wid, int hei, int r, int lang, int msg)
{
	int deltaTime = timeGetTime() - globalStartTime;
	int offset = 0.618 * r * 1.25;

	setcolor(BLACK);
	line(sx, sy + r, sx + wid, sy + r);
	setfillcolor(RED);
	solidcircle(sx + (1) * offset, sy + r / 2, 0.618 * r * 0.4);
	setfillcolor(YELLOW);
	solidcircle(sx + (2) * offset, sy + r / 2, 0.618 * r * 0.4);
	setfillcolor(GREEN);
	solidcircle(sx + (3) * offset, sy + r / 2, 0.618 * r * 0.4);

	settextstyle(r, 0, _T("Monaco"));
	setbkmode(TRANSPARENT);
	outtextxy(sx + (4.5) * 0.618 * r * 1.25, sy, "C_osu!_desu");

	const int lineLen = 32;
	char CONTENT[][lineLen] =
	{
		"Welcome to C_osu!_",	//0
		"desu, an osu! cli-",	//1
		"ent written in C!",	//2
		"*BeatMapInfo*",		//3
		" - Creator -",			//4
		"",						//5
		" - Version -",			//6
		"",						//7
		" - BeatMapID - ",		//8
		"",						//9
		"*Sorted by ID*",		//10
		"",						//11
		"L - Switch Lang",		//12
		"A/S/D - Sort",			//13
		"F/G/H - Find Next",	//14
		"Up/Down Switch BMS",	//15
		"Lft/Rt Switch BM",		//16
		"Enter - Start",		//17
		"Q - Quit"				//18
	};

	switch (lang)
	{
	case LANGUAGE_ENGLISH:
		switch (sor)
		{
		case 1:
			strcpy(CONTENT[10], "*Sorted by Artist*");
			break;
		case 2:
			strcpy(CONTENT[10], "*Sorted by Title*");
			break;
		}
		switch (msg)
		{
		case 1:
			strcpy(CONTENT[11], "**Found Match**");
			break;
		case 2:
			strcpy(CONTENT[11], "**Not Found**");
			break;
		}
		break;
	case LANGUAGE_CHINESE:
		strcpy(CONTENT[0], "欢迎来到C_osu!_desu，");
		strcpy(CONTENT[1], "又名COD(雾)，一个用C");
		strcpy(CONTENT[2], "语言实现的osu!客户端。");
		strcpy(CONTENT[3], "*乐谱信息*");
		strcpy(CONTENT[4], " - 制谱者 -");
		strcpy(CONTENT[6], " - 版本 -");
		strcpy(CONTENT[8], " - 乐谱ID -");
		switch (sor)
		{
		case 0:
			strcpy(CONTENT[10], "*已按ID排序*");
			break;
		case 1:
			strcpy(CONTENT[10], "*已按作者排序*");
			break;
		case 2:
			strcpy(CONTENT[10], "*已按标题排序*");
			break;
		}
		switch (msg)
		{
		case 1:
			strcpy(CONTENT[11], "**已跳至查找结果**");
			break;
		case 2:
			strcpy(CONTENT[11], "**未找到查找结果**");
			break;
		}
		strcpy(CONTENT[12], "L - 切换语言");
		strcpy(CONTENT[13], "A/S/D - 排序");
		strcpy(CONTENT[14], "F/G/H - 查找下一个");
		strcpy(CONTENT[15], "上/下 - 切换谱集");
		strcpy(CONTENT[16], "左/右 - 切换谱子");
		strcpy(CONTENT[17], "回车 - 开始游戏");
		strcpy(CONTENT[18], "Q - 退出");
		break;
	}


	OsuList ol = bmsl->data->ol;
	while (dif--)
		ol = ol->next;
	RearrangeString(globalBuffer, ol->data->Metadata.Creator, deltaTime, 18);
	strcpy(CONTENT[5], globalBuffer);
	RearrangeString(globalBuffer, ol->data->Metadata.Version, deltaTime, 18);
	strcpy(CONTENT[7], globalBuffer);
	sprintf(CONTENT[9], "%d", ol->data->Metadata.BeatmapID);

	for (int i = 0; i * lineLen < sizeof(CONTENT); ++i)
		outtextxy(sx + offset, sy + r / 2 + (i + 1) * offset, CONTENT[i]);
}

void ShowSlider(int Pos, int BMSNodeCnt)														//显示进度
{
	setfillcolor(0xFC7456);
	solidroundrect(596, 16, 606, 470, 10, 10);											//宽为10的条形框
	int y1, y2;
	const int tollen = 450;
	const int len = tollen / BMSNodeCnt;
	y1 = 18 + len * Pos;
	y2 = y1 + len;
	setfillcolor(BGR(0x87CEFA));
	solidroundrect(598, y1, 604, y2, 8, 8);					//所画范围为(599,18,603,468) 公式为H*(POS+1)/BMSNodeCnt

	setcolor(WHITE);
	settextstyle(20, 0, _T("Monaco"));
	sprintf(globalBuffer, "%03d", Pos + 1);
	outtextxy(598 + 20, y1 + len / 2 - 30, globalBuffer);
	outtextxy(598 + 20, y1 + len / 2 - 10, " / ");
	sprintf(globalBuffer, "%03d", BMSNodeCnt);
	outtextxy(598 + 20, y1 + len / 2 + 10, globalBuffer);

}

void RenderMainWindow(BMSList bmsl, int TriPlace, int dif, int Pos, int BMSNodeCnt, int sor, int lang, int msg)
{
	BeginBatchDraw();

	const int r = 26;
	const int sx = 26, sy = 26, ex = 250, ey = 454;

	IMAGE bg;
	loadimage(&bg, bmsl->data->backgroundPath[dif], 640, 480, true);
	//putimage(0, 0, &bg);
	setfillcolor(BLACK);
	solidrectangle(0, 0, 640, 480);
	setfillcolor(WHITE);
	solidroundrect(270, 0, 614, 480, r, r);
	solidroundrect(sx, sx, ex, ey, r, r);
	putpicture(0, 0, &bg, RGB(85, 85, 85), 100);

	DrawTextPreview(bmsl, dif, sor, sx, sy, ex - sx, ey - sy, r, lang, msg);
	InitSelectCatalogue(TriPlace);
	ShowTriangle(TriPlace);
	ShowInfoSheet(bmsl, TriPlace);
	ShowDif(bmsl, TriPlace, dif);
	ShowSlider(Pos, BMSNodeCnt);

	setcolor(WHITE);
	settextstyle(17, 0, _T("Monaco"));
	outtextxy(0, 0, "V0.9 Alpha");
	outtextxy(0, 465 - 36, "By");
	outtextxy(0, 465 - 24, "Wang");
	outtextxy(0, 465 - 12, "Yilin (aka Tang Kuku) & Zhang Haibo @ NEU");
	outtextxy(0, 465, "Copyright (c) 2022 Wang Yilin & Zhang Haibo");

	EndBatchDraw();
}

void UpdateGlobalStartTime()
{
	globalStartTime = timeGetTime();
}

const char* GetInputResult(int whatToSearch)
{
	switch (whatToSearch)
	{
	case 0:
		InputBox(globalBuffer, 1024, "查找ID:\n    模糊查找请直接输入子串，精确查找请输入在串前加'$'。\n    若找到，则会跳转；否则不动。\n\nFind ID:\nFor fuzzy search, please input the substring directly, for precise search, please put a '$' in front of the string.");
		break;
	case 1:
		InputBox(globalBuffer, 1024, "查找作者:\n    模糊查找请直接输入子串，精确查找请输入在串前加'$'。\n    若找到，则会跳转；否则不动。\n\nFind Artist:\nFor fuzzy search, please input the substring directly, for precise search, please put a '$' in front of the string.");
		break;
	case 2:
		InputBox(globalBuffer, 1024, "查找标题:\n    模糊查找请直接输入子串，精确查找请输入在串前加'$'。\n    若找到，则会跳转；否则不动。\n\nFind Title:\nFor fuzzy search, please input the substring directly, for precise search, please put a '$' in front of the string.");
		break;
	}
	return globalBuffer;
}