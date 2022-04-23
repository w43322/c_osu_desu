#pragma once

#include "global.h"
#include "osu.h"

#define NOWTIME ((int)(timeGetTime()-startTime))
#define DRAW_BEZIER(_COLOR_,_RATIO_)\
{\
	setfillcolor(##_COLOR_##);\
	setlinecolor(##_COLOR_##);\
	for(int i = 0; i < pointGroup; ++i)\
	{\
		for (int j = 0; j < BEZIERPOINTCNT; ++j)\
		{\
			fillcircle(BZPoints[i][j].x + CIRCLERADIUS, BZPoints[i][j].y + CIRCLERADIUS, CIRCLERADIUS * ##_RATIO_##);\
		}\
	}\
}

#define DRAW_ARC(_COLOR_,_RATIO_)\
{\
	setfillcolor(##_COLOR_##);\
	setlinecolor(##_COLOR_##);\
	for (int j = 0; j < BEZIERPOINTCNT; ++j)\
	{\
		fillcircle(ArcPoints[j].x + CIRCLERADIUS, ArcPoints[j].y + CIRCLERADIUS, CIRCLERADIUS * ##_RATIO_##);\
	}\
}

#define DRAW_CIRCLE(_COLOR_,_RATIO_,_OFFSET_X_,_OFFSET_Y_)\
{\
	setfillcolor(##_COLOR_##);\
	setlinecolor(##_COLOR_##);\
	fillcircle(thisHitObject->x + CIRCLERADIUS + (##_OFFSET_X_##), thisHitObject->y + CIRCLERADIUS + (##_OFFSET_Y_##) , CIRCLERADIUS * ##_RATIO_##);\
}

float Dist(POINT A, POINT B);
int Combinition(int N, int M); // N up to 5
POINT Bezier(const POINT* Points, int N, float T);
POINT GetCircleCentre(POINT start, POINT mid, POINT end);
POINT Circle(const POINT* Points, float T);
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color, int alpha);
void printCircle(struct _HITOBJECTS* thisHitObject, int nowTime, int preempt, int overrideX = -1, int overrideY = -1, int isReverseCircle = 0, int approachCircleRadiusOverride = -1);
void printBazier(struct _HITOBJECTS* thisHitObject, int nowTime, int preempt, int timeSpan);
void printArc(struct _HITOBJECTS* thisHitObject, int nowTime, int preempt, int timeSpan);
void DrawCurser(int X, int Y);
void printInfo(float F, int C, int S);
void StopMusic();
void PlayMusic(const char* PATH, int previewTime);
void RunGameFromOsuFile(Osu* osuFilePointer);