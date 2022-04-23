#pragma once

#include "global.h"
#include "osu.h"
#include "osz.h"
#include "game.h"

typedef struct OSULIST
{
	Osu* data;
	struct OSULIST* next;
}*OsuList;
typedef struct BMS_STRUCT
{
	char id[16];
	char title[256];
	char artist[256];
	char audioPath[256];
	char backgroundPath[16][256];
	int osuFileCnt;
	OsuList ol;
}BMS;
typedef struct BMSLIST
{
	BMS* data;
	struct BMSLIST* prev;
	struct BMSLIST* next;
}*BMSList;

void GetOsuFileList(BMSList* bmsl);
void BMSListInit(BMSList* bmsl, int* cnt);
void PrintBMSList(BMSList bmsl);
void SortBMSListByArtist(BMSList* bmsl);
void SortBMSListByTitle(BMSList* bmsl);
void SortBMSListById(BMSList* bmsl);
void ChangeSortingMethod(BMSList* bmsl, int* sor, int to, int* TriPlace, int* dif, int* sliderPos);
void FindByArtist(BMSList* bmsl, const char* searchStr, int* pos, int cnt, int* msg, int* TriPlace, int* dif);
void FindByTitle(BMSList* bmsl, const char* searchStr, int* pos, int cnt, int* msg, int* TriPlace, int* dif);
void FindByID(BMSList* bmsl, const char* searchStr, int* pos, int cnt, int* msg, int* TriPlace, int* dif);
void StartMainMenu();