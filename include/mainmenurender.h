#pragma once

#include "global.h"
#include "mainmenulogic.h"

void RearrangeString(char* res, const char* STR, int eTime, int maxLen); // maxLen <= 32
void OutPutString(int x1, int y1, int x2, int y2, const char* arr);								//居中输出字符串
void InitSelectCatalogue(int Place);								//选择界面 固定每个选项窗口大小为244*60 起始x坐标为320 共五个窗口
void ShowCatalogueBox(BMSList B, int Place);
void ShowInfoSheet(BMSList B, int Place);
void JudgeTriangleP(int* Place);
void ShowDif(BMSList B, int Place, int dif);
void JudgeDif(int* dif, BMSList B);
void DrawTextPreview(BMSList bmsl, int dif, int sor, int sx, int sy, int wid, int hei, int r, int lang, int msg);
void ShowSlider(int Pos, int BMSNodeCnt);
void RenderMainWindow(BMSList bmsl, int TriPlace, int dif, int Pos, int BMSNodeCnt, int sor, int lang, int msg);
void UpdateGlobalStartTime();
const char* GetInputResult(int whatToSearch);