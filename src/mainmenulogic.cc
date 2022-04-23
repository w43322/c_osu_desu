#include "mainmenulogic.h"
#include "mainmenurender.h"

void GetOsuFileList(BMSList* bmsl)
{
	(*bmsl)->data->osuFileCnt = 0;
	(*bmsl)->data->ol = NULL;

	DIR* d;
	struct dirent* dir;
	char path[256] = ".\\beatmapsets\\";
	strcat(path, (*bmsl)->data->id);
	strcat(path, "\\");
	d = opendir(path);

	OsuList oldOLNode = NULL;

	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			char* rdot = strrchr(dir->d_name, '.');
			if (rdot && strcmp(rdot, ".osu") == 0)
			{
				printf("osu file read:%s\n", dir->d_name);

				OsuList newOLNode = (OsuList)malloc(sizeof(struct OSULIST));
				newOLNode->data = (Osu*)malloc(sizeof Osu);
				if ((*bmsl)->data->osuFileCnt == 0)
					(*bmsl)->data->ol = newOLNode;
				else
				{
					//Copy Color Combo From First
					memcpy(&newOLNode->data->Colours, &oldOLNode->data->Colours, sizeof(struct _COLOURS));

					oldOLNode->next = newOLNode;
				}

				char osuPath[256];
				strcpy(osuPath, path);
				strcat(osuPath, dir->d_name);

				ReadOsuFile(newOLNode->data, osuPath, 0);

				strcpy(osuPath, path);
				strcat(osuPath, newOLNode->data->Events.BackgroundFilename);
				//printf("bgpath:%s\n", osuPath);
				strcpy((*bmsl)->data->backgroundPath[(*bmsl)->data->osuFileCnt], osuPath);

				oldOLNode = newOLNode;
				++(*bmsl)->data->osuFileCnt;
			}
		}
		closedir(d);
	}

	oldOLNode->next = NULL;
	strcpy((*bmsl)->data->artist, (*bmsl)->data->ol->data->Metadata.Artist);
	strcpy((*bmsl)->data->title, (*bmsl)->data->ol->data->Metadata.Title);
	strcpy((*bmsl)->data->audioPath, path);
	strcat((*bmsl)->data->audioPath, (*bmsl)->data->ol->data->General.AudioFilename);
}

void BMSListInit(BMSList* bmsl, int* cnt)
{
	DIR* d;
	struct dirent* dir;
	d = opendir(".\\beatmapsets");

	FileList* idList = (FileList*)malloc(sizeof(struct FILELIST));
	strcpy(idList->path, "$HEAD$");
	idList->next = NULL;

	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (0 != strcmp("..", dir->d_name) && 0 != strcmp(".", dir->d_name))
				FileListPushBack(idList, dir->d_name);
		}
		closedir(d);
	}

	BMSList bmslist = NULL;

	FileList* nowNode = idList->next;
	BMSList nowBMSListNode = NULL;

	while (nowNode)
	{
		BMSList newBMSListNode = (BMSList)malloc(sizeof(struct BMSLIST));
		if (bmslist)
		{
			nowBMSListNode->next = newBMSListNode;
			newBMSListNode->prev = nowBMSListNode;
		}
		else
		{
			bmslist = newBMSListNode;
			nowBMSListNode = bmslist;
		}

		printf("id:%s\n", nowNode->path);
		newBMSListNode->data = (BMS*)malloc(sizeof(struct BMS_STRUCT));
		strcpy(newBMSListNode->data->id, nowNode->path);
		printf("copyid:%s\n", newBMSListNode->data->id);

		nowNode = nowNode->next;
		nowBMSListNode = newBMSListNode;
	}

	bmslist->prev = nowBMSListNode;
	nowBMSListNode->next = bmslist;

	*cnt = 1;
	GetOsuFileList(&bmslist);
	nowBMSListNode = bmslist->next;
	while (nowBMSListNode != bmslist)
	{
		GetOsuFileList(&nowBMSListNode);
		nowBMSListNode = nowBMSListNode->next;
		++* cnt;
	}



	*bmsl = bmslist;
}

void PrintBMSList(BMSList bmsl)
{
	BMSList nowBMSListNode = bmsl;
	int vis = 0;
	while (nowBMSListNode != bmsl || !vis)
	{
		if (vis == 0)vis = 1;
		printf("id:%s artist:%s title:%s\n", nowBMSListNode->data->id, nowBMSListNode->data->artist, nowBMSListNode->data->title);
		printf("audioPath:%s\n", nowBMSListNode->data->audioPath);
		nowBMSListNode = nowBMSListNode->next;
	}
}

void SortBMSListByArtist(BMSList* bmsl)
{
	BMSList first = *bmsl;
	BMSList last = (*bmsl)->prev;
	int vis = 0;
	for (BMSList* i = &first; *i != first || vis == 0; i = &(*i)->next)
	{
		if (vis == 0)
			vis = 1;
		for (BMSList* j = i; *j != first; j = &(*j)->prev)
		{
			if (strcmp((*j)->prev->data->artist, (*j)->data->artist) > 0)
			{
				BMS* tmp = (*j)->prev->data;
				(*j)->prev->data = (*j)->data;
				(*j)->data = tmp;
			}
		}
	}
}

void SortBMSListByTitle(BMSList* bmsl)
{
	BMSList first = *bmsl;
	BMSList last = (*bmsl)->prev;
	int vis = 0;
	for (BMSList* i = &first; *i != first || vis == 0; i = &(*i)->next)
	{
		if (vis == 0)
			vis = 1;
		for (BMSList* j = i; *j != first; j = &(*j)->prev)
		{
			if (strcmp((*j)->prev->data->title, (*j)->data->title) > 0)
			{
				BMS* tmp = (*j)->prev->data;
				(*j)->prev->data = (*j)->data;
				(*j)->data = tmp;
			}
		}
	}
}

void SortBMSListById(BMSList* bmsl)
{
	BMSList first = *bmsl;
	BMSList last = (*bmsl)->prev;
	int vis = 0;
	for (BMSList* i = &first; *i != first || vis == 0; i = &(*i)->next)
	{
		if (vis == 0)
			vis = 1;
		for (BMSList* j = i; *j != first; j = &(*j)->prev)
		{
			if (atoi((*j)->prev->data->id) > atoi((*j)->data->id))
			{
				BMS* tmp = (*j)->prev->data;
				(*j)->prev->data = (*j)->data;
				(*j)->data = tmp;
			}
		}
	}
}

void ChangeSortingMethod(BMSList* bmsl, int* sor, int to, int* TriPlace, int* dif, int* sliderPos)
{
	*TriPlace = 1;
	*dif = 0;
	*sliderPos = 0;
	*sor = to;
	switch (*sor)
	{
	case 0:
		SortBMSListById(bmsl);
		break;
	case 1:
		SortBMSListByArtist(bmsl);
		break;
	case 2:
		SortBMSListByTitle(bmsl);
		break;
	}
}

void FindByArtist(BMSList* bmsl, const char* searchStr, int* pos, int cnt, int* msg, int* TriPlace, int* dif)
{
	*bmsl = (*bmsl)->next;
	*pos = (*pos + 1) % cnt;
	int vis = 0;
	BMSList first = *bmsl;
	while (*bmsl != first || vis == 0)
	{
		if (!vis)
			vis = 1;
		switch (searchStr[0])
		{
		case '\0':
			printf("empty string, returning\n");
			*bmsl = (*bmsl)->prev;
			*pos = (*pos - 1 + cnt) % cnt;
			*msg = 2;
			return;
		case '$':
			printf("attemping exact match:%s:%s\n", (*bmsl)->data->artist, searchStr + 1);
			if (0 == strcmp((*bmsl)->data->artist, searchStr + 1))
			{
				printf("exact match found:%s:%s\n", (*bmsl)->data->artist, searchStr + 1);
				*msg = 1;
				*TriPlace = 3;
				PlayMusic((*bmsl)->data->audioPath, (*bmsl)->data->ol->data->General.PreviewTime);
				*dif = 0;
				return;
			}
			break;
		default:
			printf("attemping match:%s:%s\n", (*bmsl)->data->artist, searchStr);
			if (strstr((*bmsl)->data->artist, searchStr) != NULL)
			{
				printf("match found:%s:%s\n", (*bmsl)->data->artist, searchStr);
				*msg = 1;
				*TriPlace = 3;
				PlayMusic((*bmsl)->data->audioPath, (*bmsl)->data->ol->data->General.PreviewTime);
				*dif = 0;
				return;
			}
		}
		*bmsl = (*bmsl)->next;
		*pos = (*pos + 1) % cnt;
	}
	*bmsl = (*bmsl)->prev;
	*pos = (*pos - 1 + cnt) % cnt;
	*msg = 2;
}

void FindByTitle(BMSList* bmsl, const char* searchStr, int* pos, int cnt, int* msg, int* TriPlace, int* dif)
{
	*bmsl = (*bmsl)->next;
	*pos = (*pos + 1) % cnt;
	int vis = 0;
	BMSList first = *bmsl;
	while (*bmsl != first || vis == 0)
	{
		if (!vis)
			vis = 1;
		switch (searchStr[0])
		{
		case '\0':
			printf("empty string, returning\n");
			*bmsl = (*bmsl)->prev;
			*pos = (*pos - 1 + cnt) % cnt;
			*msg = 2;
			return;
		case '$':
			printf("attemping exact match:%s:%s\n", (*bmsl)->data->title, searchStr + 1);
			if (0 == strcmp((*bmsl)->data->title, searchStr + 1))
			{
				printf("exact match found:%s:%s\n", (*bmsl)->data->title, searchStr + 1);
				*msg = 1;
				*TriPlace = 3;
				PlayMusic((*bmsl)->data->audioPath, (*bmsl)->data->ol->data->General.PreviewTime);
				*dif = 0;
				return;
			}
			break;
		default:
			printf("attemping match:%s:%s\n", (*bmsl)->data->title, searchStr);
			if (strstr((*bmsl)->data->title, searchStr) != NULL)
			{
				printf("match found:%s:%s\n", (*bmsl)->data->title, searchStr);
				*msg = 1;
				*TriPlace = 3;
				PlayMusic((*bmsl)->data->audioPath, (*bmsl)->data->ol->data->General.PreviewTime);
				*dif = 0;
				return;
			}
		}
		*bmsl = (*bmsl)->next;
		*pos = (*pos + 1) % cnt;
	}
	*bmsl = (*bmsl)->prev;
	*pos = (*pos - 1 + cnt) % cnt;
	*msg = 2;
}

void FindByID(BMSList* bmsl, const char* searchStr, int* pos, int cnt, int* msg, int* TriPlace, int* dif)
{
	*bmsl = (*bmsl)->next;
	*pos = (*pos + 1) % cnt;
	int vis = 0;
	BMSList first = *bmsl;
	while (*bmsl != first || vis == 0)
	{
		if (!vis)
			vis = 1;
		switch (searchStr[0])
		{
		case '\0':
			printf("empty string, returning\n");
			*bmsl = (*bmsl)->prev;
			*pos = (*pos - 1 + cnt) % cnt;
			*msg = 2;
			return;
		case '$':
			printf("attemping exact match:%s:%s\n", (*bmsl)->data->id, searchStr + 1);
			if (0 == strcmp((*bmsl)->data->id, searchStr + 1))
			{
				printf("exact match found:%s:%s\n", (*bmsl)->data->id, searchStr + 1);
				*msg = 1;
				*TriPlace = 3;
				PlayMusic((*bmsl)->data->audioPath, (*bmsl)->data->ol->data->General.PreviewTime);
				*dif = 0;
				return;
			}
			break;
		default:
			printf("attemping match:%s:%s\n", (*bmsl)->data->id, searchStr);
			if (strstr((*bmsl)->data->id, searchStr) != NULL)
			{
				printf("match found:%s:%s\n", (*bmsl)->data->id, searchStr);
				*msg = 1;
				*TriPlace = 3;
				PlayMusic((*bmsl)->data->audioPath, (*bmsl)->data->ol->data->General.PreviewTime);
				*dif = 0;
				return;
			}
		}
		*bmsl = (*bmsl)->next;
		*pos = (*pos + 1) % cnt;
	}
	*bmsl = (*bmsl)->prev;
	*pos = (*pos - 1 + cnt) % cnt;
	*msg = 2;
}
void StartMainMenu()
{
	initgraph(640, 480/*, EW_SHOWCONSOLE*/);// 初始化图形窗口
	IMAGE bg;
	loadimage(&bg, ".\\logo.png", 640, 480, true);
	putpicture(0, 0, &bg, RGB(85, 85, 85), 255);
	Extract();

	int BMSNodeCnt;
	BMSList bmsl;
	BMSListInit(&bmsl, &BMSNodeCnt);
	SortBMSListById(&bmsl);
	PrintBMSList(bmsl);

	UpdateGlobalStartTime();

								

	char tempChar;
	int TriPlace = 1;
	int dif = 0;
	int sliderPos = 0;
	int sortingMethod = 0;
	int langID = 1;
	int displayMsg = 0;

	PlayMusic(bmsl->data->audioPath, bmsl->data->ol->data->General.PreviewTime);
	while (1)
	{
		RenderMainWindow(bmsl, TriPlace, dif, sliderPos, BMSNodeCnt, sortingMethod, langID, displayMsg);
		if (_kbhit())
		{
			tempChar = _getch();
			switch (tempChar)
			{
			case KB_KEY_ENTER:
			{
				OsuList ol = bmsl->data->ol;
				for (int i = 0; i < dif; ++i)
					ol = ol->next;
				printf("Entering Game : ID:%s, Title:%s, %d th osu file\n", bmsl->data->id, bmsl->data->title, dif + 1);
				RunGameFromOsuFile(ol->data);
				break;
			}
			case KB_KEY_A_1:case KB_KEY_A_2:
				ChangeSortingMethod(&bmsl, &sortingMethod, 0, &TriPlace, &dif, &sliderPos);
				break;
			case KB_KEY_S_1:case KB_KEY_S_2:
				ChangeSortingMethod(&bmsl, &sortingMethod, 1, &TriPlace, &dif, &sliderPos);
				break;
			case KB_KEY_D_1:case KB_KEY_D_2:
				ChangeSortingMethod(&bmsl, &sortingMethod, 2, &TriPlace, &dif, &sliderPos);
				break;
			case KB_KEY_F_1:case KB_KEY_F_2:
				FindByID(&bmsl, GetInputResult(1), &sliderPos, BMSNodeCnt, &displayMsg, &TriPlace, &dif);
				break;
			case KB_KEY_G_1:case KB_KEY_G_2:
				FindByArtist(&bmsl, GetInputResult(2), &sliderPos, BMSNodeCnt, &displayMsg, &TriPlace, &dif);
				break;
			case KB_KEY_H_1:case KB_KEY_H_2:
				FindByTitle(&bmsl, GetInputResult(3), &sliderPos, BMSNodeCnt, &displayMsg, &TriPlace, &dif);
				break;
			case KB_KEY_L_1:case KB_KEY_L_2:
				langID ^= 1;
				break;
			case KB_KEY_Q_1:case KB_KEY_Q_2:
				return;
			case KB_KEY_DIRECTIONAL:
				char kbhit = _getch();
				switch (kbhit)
				{
				case KB_KEY_UP:											//方向上键
					TriPlace--;
					sliderPos = (sliderPos - 1 + BMSNodeCnt) % BMSNodeCnt;
					JudgeTriangleP(&TriPlace);
					dif = 0;
					displayMsg = 0;
					bmsl = bmsl->prev;
					PlayMusic(bmsl->data->audioPath, bmsl->data->ol->data->General.PreviewTime);
					break;
				case KB_KEY_DOWN:											//方向下键
					TriPlace++;
					sliderPos = (sliderPos + 1) % BMSNodeCnt;
					JudgeTriangleP(&TriPlace);
					dif = 0;
					displayMsg = 0;
					bmsl = bmsl->next;
					PlayMusic(bmsl->data->audioPath, bmsl->data->ol->data->General.PreviewTime);
					break;
				case KB_KEY_LEFT:
					dif--;
					JudgeDif(&dif, bmsl);
					break;
				case KB_KEY_RIGHT:
					dif++;
					JudgeDif(&dif, bmsl);
					break;
				}
				break;
			}

		}

	}


	// 关闭图形窗口
	closegraph();
}