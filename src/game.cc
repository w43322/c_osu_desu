#include "game.h"

float Dist(POINT A, POINT B)
{
	float dx = A.x - B.x;
	float dy = A.y - B.y;
	return sqrt(dx * dx + dy * dy);
}

int Combinition(int N, int M) // N up to 5
{
	switch (N)
	{
	case 1:
		switch (M)
		{
		case 0:case 1:return 1;
		default:return 0;
		}
		break;
	case 2:
		switch (M)
		{
		case 0:case 2:return 1;
		case 1:return 2;
		default:return 0;
		}
		break;
	case 3:
		switch (M)
		{
		case 0:case 3:return 1;
		case 1:case 2:return 3;
		default:return 0;
		}
		break;
	case 4:
		switch (M)
		{
		case 0:case 4:return 1;
		case 1:case 3:return 4;
		case 2:return 6;
		default:return 0;
		}
	case 5:
		switch (M)
		{
		case 0:case 5:return 1;
		case 1:case 4:return 5;
		case 2:case 3:return 10;
		default:return 0;
		}
	default:return 0;
	}
}

POINT Bezier(const POINT* Points, int N, float T)
{
	if (T < 0.0f)T = 0.0f;
	if (T > 1.0f)T = 1.0f;

	POINT res;
	float sum_x = 0.0f, sum_y = 0.0f;
	for (int i = 0; i <= N; ++i)
	{
		sum_x += pow(1 - T, N - i) * pow(T, i) * Combinition(N, i) * Points[i].x;
		sum_y += pow(1 - T, N - i) * pow(T, i) * Combinition(N, i) * Points[i].y;
	}
	res.x = (long)sum_x, res.y = (long)sum_y;
	return res;
}

POINT GetCircleCentre(POINT start, POINT mid, POINT end)
{
	float x1 = 0.5 * (start.x + mid.x);
	float y1 = 0.5 * (start.y + mid.y);
	float x2 = 0.5 * (end.x + mid.x);
	float y2 = 0.5 * (end.y + mid.y);
	float x0 = x1 + (y1 - start.y);
	float y0 = y1 - (x1 - start.x);
	float x3 = x2 + (y2 - end.y);
	float y3 = y2 - (x2 - end.x);
	float a, b, c, d, e, f;
	float dx01 = x1 - x0;
	float dy01 = y1 - y0;
	float dx23 = x3 - x2;
	float dy23 = y3 - y2;
	a = dy01;
	b = -dx01;
	c = dy01 * x0 - dx01 * y0;
	d = dy23;
	e = -dx23;
	f = dy23 * x2 - dx23 * y2;
	if (fabs(a) < EPS)
	{
		a += d; b += e; c += f;
	}
	float tmp = d / a;
	d -= d * tmp; e -= b * tmp; f -= c * tmp;
	tmp = b / e;
	b -= e * tmp; c -= f * tmp;
	POINT res;
	res.x = c / a;
	res.y = f / e;
	res.y *= -1;
	return res;
}
POINT Circle(const POINT* Points, float T)
{
	POINT start = Points[0];
	POINT mid = Points[1];
	POINT end = Points[2];
	float startx = (float)start.x, starty = (float)start.y;
	float midx = (float)mid.x, midy = (float)mid.y;
	float endx = (float)end.x, endy = (float)end.y;
	starty *= -1, midy *= -1, endy *= -1;
	float temp;
	temp = (midx - startx) * (endx - midx) + (midy - starty) * (endy - midy);
	int flag = temp > 0 ? 1 : -1;                             //flag:1为劣弧（圆心角小于180） -1为优弧
	int judge = 1;
	POINT centre = GetCircleCentre(start, mid, end);
	float centrex = (float)centre.x, centrey = (float)centre.y;
	float cross1 = ((centrex - startx) * (centrey - endy) - (centrex - endx) * (centrey - starty));
	float cross2 = ((centrex - midx) * (centrey - endy) - (centrex - endx) * (centrey - midy));
	if (cross1 > 0 || cross2 > 0)
		judge = -1;
	int dir = flag * judge;                              	 //圆的方向 顺时针返回1 逆时针返回-1
	if (T < 0.0f)T = 0.0f;
	if (T > 1.0f)T = 1.0f;
	float LenSquared = (endx - startx) * (endx - startx) + (endy - starty) * (endy - starty);
	float Radius = sqrt((centrex - startx) * (centrex - startx) + (centrey - starty) * (centrey - starty));
	float COSX = 1 - LenSquared / (2 * Radius * Radius);
	float result = acos(COSX);
	if (flag < 0)   result = 2 * PI - result;
	float angle = T * result;
	POINT res;
	float S_COS = (startx - centrex) / Radius;
	float S_SIN = (starty - centrey) / Radius;
	float S_angle;
	if (S_COS == 0)
		S_angle = PI/2;
	else if (S_COS == 1)
		S_angle = 0;
	else
	{
		if (S_COS > 0 && S_SIN > 0)
			S_angle = acos(S_COS);
		if (S_COS > 0 && S_SIN < 0)
			S_angle = 2 * PI - acos(S_COS);
		if (S_COS < 0 && S_SIN>0)
			S_angle = acos(S_COS);
		if (S_COS < 0 && S_SIN < 0)
			S_angle = 2 * PI - acos(S_COS);
	}
	res.x = centrex + Radius * cos(S_angle - dir * angle);
	res.y = -1 * (centrey + Radius * sin(S_angle - dir * angle));
	return res;
}

void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color, int alpha) 
{//0~255 255表示不透明
	DWORD* imgp = GetImageBuffer(img);
	DWORD* bgimgp = GetImageBuffer();
	int w, bw, h, i, j;
	w = img->getwidth();
	bw = getwidth();
	h = img->getheight();
	color += 0xff000000;
	if (alpha < 0)alpha = 0;
	else if (alpha > 255)alpha = 255;
	for (i = 0; i < h; ++i)
		for (j = 0; j < w; ++j)
			if (imgp[i * w + j] != color)
				bgimgp[(i + dsty) * bw + j + dstx] = RGB(
					((int)(alpha / 255.0 * GetRValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetRValue(bgimgp[(i + dsty) * bw + j + dstx]))),
					((int)(alpha / 255.0 * GetGValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetGValue(bgimgp[(i + dsty) * bw + j + dstx]))),
					((int)(alpha / 255.0 * GetBValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetBValue(bgimgp[(i + dsty) * bw + j + dstx])))
				);
}

void printCircle(struct _HITOBJECTS* thisHitObject, int nowTime, int preempt, int overrideX, int overrideY, int isReverseCircle, int approachCircleRadiusOverride)
{
	COLORREF cr = RGB(thisHitObject->_COLOUR[0], thisHitObject->_COLOUR[1], thisHitObject->_COLOUR[2]);
	int Brightness = (0.299 * thisHitObject->_COLOUR[0] + 0.587 * thisHitObject->_COLOUR[1] + 0.114 * thisHitObject->_COLOUR[2]);
	COLORREF inv = Brightness > 0.5 ? BLACK : WHITE;

	int offsetX = overrideX == -1 ? 0 : overrideX - thisHitObject->x;
	int offsetY = overrideY == -1 ? 0 : overrideY - thisHitObject->y;

	// Draw Shadow
	DRAW_CIRCLE(SHADOWCOLOR, SHADOWRATIO, offsetX, offsetY)

		// Draw Border
		DRAW_CIRCLE(BORDERCOLOR, 1.0f, offsetX, offsetY)

		// Draw Inner Shadow
		DRAW_CIRCLE(SHADOWCOLOR, INNER_SHADOWRATIO, offsetX, offsetY)

		// Draw a Circle
		DRAW_CIRCLE(cr, CIRCLERATIO, offsetX, offsetY)

		// Put Combo Number
		settextcolor(inv);
	settextstyle(FONTHEIGHT, FONTWIDTH, _T("Bodoni MT"));
	outtextxy(offsetX + thisHitObject->x + CIRCLERADIUS - FONTWIDTH / 2,
		offsetY + thisHitObject->y + CIRCLERADIUS - FONTHEIGHT / 2, isReverseCircle ? '^' : thisHitObject->_COMBO);

	// Draw Approach Circle
	if (!isReverseCircle)
	{
		int remainingTime = thisHitObject->time - nowTime;
		int approachCircleRadius;
		if (remainingTime > 0)
			approachCircleRadius = CIRCLERADIUS + APPROACHCIRCLERADIUS * remainingTime / preempt;
		else
			approachCircleRadius = approachCircleRadiusOverride == -1 ? (CIRCLERADIUS * AFTER_CLICK_RATIO) : approachCircleRadiusOverride;
		setlinestyle(PS_SOLID, LINEWIDTH);
		circle(offsetX + thisHitObject->x + CIRCLERADIUS,
			offsetY + thisHitObject->y + CIRCLERADIUS, approachCircleRadius);
		setlinestyle(PS_SOLID, 1);
	}
}

void printBazier(struct _HITOBJECTS* thisHitObject, int nowTime, int preempt, int timeSpan)
{
	COLORREF cr = RGB(thisHitObject->_COLOUR[0], thisHitObject->_COLOUR[1], thisHitObject->_COLOUR[2]);
	COLORREF inv = RGB(-thisHitObject->_COLOUR[0], -thisHitObject->_COLOUR[1], -thisHitObject->_COLOUR[2]);

	POINT Points[16][32];
	int pointCntInGroup[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	float disOfEachGroup[16] = { 0.0f , 0.0f , 0.0f, 0.0f,0.0f , 0.0f , 0.0f, 0.0f ,0.0f , 0.0f , 0.0f, 0.0f ,0.0f , 0.0f , 0.0f, 0.0f };
	float lengthRatioOfEachGroup[16] = { 0.0f, 0.0f , 0.0f, 0.0f,0.0f , 0.0f , 0.0f, 0.0f ,0.0f , 0.0f , 0.0f, 0.0f ,0.0f , 0.0f , 0.0f, 0.0f };
	POINT BZPoints[16][BEZIERPOINTCNT];

	// Pre Process
	Points[0][0].x = thisHitObject->x, Points[0][0].y = thisHitObject->y;
	int pointGroup = 0, nowPoint = 0;
	while (nowPoint < thisHitObject->objectParams.slider.curvePointsCnt)
	{
		while (nowPoint < thisHitObject->objectParams.slider.curvePointsCnt
			&& (thisHitObject->objectParams.slider.curvePoints[nowPoint][0] != Points[pointGroup][pointCntInGroup[pointGroup] - 1].x
				|| thisHitObject->objectParams.slider.curvePoints[nowPoint][1] != Points[pointGroup][pointCntInGroup[pointGroup] - 1].y))
		{
			Points[pointGroup][pointCntInGroup[pointGroup]].x = thisHitObject->objectParams.slider.curvePoints[nowPoint][0];
			Points[pointGroup][pointCntInGroup[pointGroup]].y = thisHitObject->objectParams.slider.curvePoints[nowPoint][1];
			++pointCntInGroup[pointGroup], ++nowPoint;
		}
		++pointGroup;
		Points[pointGroup][0].x = thisHitObject->objectParams.slider.curvePoints[nowPoint][0];
		Points[pointGroup][0].y = thisHitObject->objectParams.slider.curvePoints[nowPoint][1];
		++nowPoint;
	}

	// Calculate Bezier Points & Length of each Segment
	float totLength = 0.0f;
	for (int i = 0; i < pointGroup; ++i)
	{
		//for (int j = 0; j < pointCntInGroup[i]; ++j)
			//printf("Points[%d][%d]: x:%d,y:%d\n", i, j, Points[i][j].x, Points[i][j].y);
		BZPoints[i][0] = Points[i][0];
		int j = 1;
		for (float p = BEZIERSTEP; j < BEZIERPOINTCNT; p += BEZIERSTEP, ++j)
		{
			BZPoints[i][j] = Bezier(Points[i], pointCntInGroup[i] - 1, p);
			disOfEachGroup[i] += Dist(BZPoints[i][j], BZPoints[i][j - 1]);
		}
		totLength += disOfEachGroup[i];
	}
	for (int i = 0; i < pointGroup; ++i)
		lengthRatioOfEachGroup[i] = disOfEachGroup[i] / totLength;

	// Pass 1
	// Draw Shadow
	DRAW_BEZIER(SHADOWCOLOR, SHADOWRATIO)

		// Pass 2
		// Draw Border
		DRAW_BEZIER(BORDERCOLOR, 1.0f)

		// Pass 3
		// Draw Inner Shadow Border
		DRAW_BEZIER(SHADOWCOLOR, INNER_SHADOWRATIO)

		// Pass 4
		// Draw Inner
		DRAW_BEZIER(cr, CIRCLERATIO)

		// Draw a Circle
		// Get Back-and-Forth Count
		int backAndForthCnt = thisHitObject->objectParams.slider.slides;
	timeSpan *= backAndForthCnt;

	float nowProgress = (float)(nowTime - thisHitObject->time) / timeSpan;
	if (nowProgress < 0.0f)nowProgress = 0.0f;
	if (nowProgress > 1.0f)nowProgress = 1.0f;
	float adjustedProgress = nowProgress * backAndForthCnt;
	int nowPass = 1;
	while (adjustedProgress > 1.0f)
	{
		++nowPass;
		adjustedProgress -= 1.0f;
	}
	if (nowPass % 2 == 0)
		adjustedProgress = 1.0f - adjustedProgress;

	// Get Which Group is the Circle In
	int progressIsInGroup = 0;
	while (progressIsInGroup < pointGroup - 1 && adjustedProgress - lengthRatioOfEachGroup[progressIsInGroup] > 0.0f)
	{
		adjustedProgress -= lengthRatioOfEachGroup[progressIsInGroup];
		++progressIsInGroup;
	}
	POINT cir = Bezier(Points[progressIsInGroup], pointCntInGroup[progressIsInGroup] - 1, adjustedProgress / lengthRatioOfEachGroup[progressIsInGroup]);
	printCircle(thisHitObject, nowTime, preempt, cir.x, cir.y, 0, APPROACHCIRCLERADIUS);

	// Print Reverse Circle
	if (backAndForthCnt > 1)
	{
		cir = Bezier(Points[pointGroup - 1], pointCntInGroup[pointGroup - 1] - 1, 1.0f);
		printCircle(thisHitObject, nowTime, preempt, cir.x, cir.y, 1);
	}

}

void printArc(struct _HITOBJECTS* thisHitObject, int nowTime, int preempt, int timeSpan)
{
	COLORREF cr = RGB(thisHitObject->_COLOUR[0], thisHitObject->_COLOUR[1], thisHitObject->_COLOUR[2]);
	COLORREF inv = RGB(-thisHitObject->_COLOUR[0], -thisHitObject->_COLOUR[1], -thisHitObject->_COLOUR[2]);

	float length = 0.0f;
	POINT ArcPoints[BEZIERPOINTCNT];
	POINT Points[3];

	// Pre Process
	Points[0].x = thisHitObject->x;
	Points[0].y = thisHitObject->y;
	Points[1].x = thisHitObject->objectParams.slider.curvePoints[0][0];
	Points[1].y = thisHitObject->objectParams.slider.curvePoints[0][1];
	Points[2].x = thisHitObject->objectParams.slider.curvePoints[1][0];
	Points[2].y = thisHitObject->objectParams.slider.curvePoints[1][1];


	// Calculate Bezier Points & Length of each Segment
	for (int j = 0; j < 3; ++j)
		printf("Points[%d]: x:%d,y:%d\n", j, Points[j].x, Points[j].y);
	ArcPoints[0] = Points[0];
	int j = 1;
	for (float p = BEZIERSTEP; j < BEZIERPOINTCNT; p += BEZIERSTEP, ++j)
	{
		ArcPoints[j] = Circle(Points, p);
		length += Dist(ArcPoints[j], ArcPoints[j - 1]);
	}

	// Pass 1
	// Draw Shadow
	DRAW_ARC(SHADOWCOLOR, SHADOWRATIO)

		// Pass 2
		// Draw Border
		DRAW_ARC(BORDERCOLOR, 1.0f)

		// Pass 3
		// Draw Inner Shadow Border
		DRAW_ARC(SHADOWCOLOR, INNER_SHADOWRATIO)

		// Pass 4
		// Draw Inner
		DRAW_ARC(cr, CIRCLERATIO)

		// Draw a Circle
		// Get Back-and-Forth Count
		int backAndForthCnt = thisHitObject->objectParams.slider.slides;
	timeSpan *= backAndForthCnt;

	float nowProgress = (float)(nowTime - thisHitObject->time) / timeSpan;
	if (nowProgress < 0.0f)nowProgress = 0.0f;
	if (nowProgress > 1.0f)nowProgress = 1.0f;
	float adjustedProgress = nowProgress * backAndForthCnt;
	int nowPass = 1;
	while (adjustedProgress > 1.0f)
	{
		++nowPass;
		adjustedProgress -= 1.0f;
	}
	if (nowPass % 2 == 0)
		adjustedProgress = 1.0f - adjustedProgress;


	POINT cir = Circle(Points, adjustedProgress);
	//printf("target x:%f targety:%f\n", cir.x, cir.y);
	printCircle(thisHitObject, nowTime, preempt, cir.x, cir.y, 0, APPROACHCIRCLERADIUS);

	// Print Reverse Circle
	if (backAndForthCnt > 1)
	{
		cir = Circle(Points, 1.0f);
		printCircle(thisHitObject, nowTime, preempt, cir.x, cir.y, 1);
	}

}

void DrawCurser(int X, int Y)
{
	setfillcolor(BLACK);
	fillcircle(X, Y, 10);
}

void printInfo(float F, int C, int S)
{
	char buffer[128];
	sprintf(buffer, _T("Accuracy : %.2f%% Combo : %d Score : %d"), F * 100.0f, C, S);
	settextcolor(WHITE);
	settextstyle(FONTHEIGHT, 0, _T("Monaco"));
	outtextxy(0, 0, buffer);
}

void StopMusic()
{
	mciSendString("close BGM", nullptr, 0, nullptr);
}

void PlayMusic(const char* PATH, int previewTime)
{
	StopMusic();
	char buf[512] = "open \"";
	strcat(buf, PATH);
	strcat(buf, "\" alias BGM");//_T("open ./beatmapsets/1525731/audio.mp3 alias BGM")
	mciSendString(buf, nullptr, 0, nullptr);
	printf("string:%s\n", buf);
	if (previewTime != -1)
	{
		sprintf(buf, "seek BGM to %d", previewTime);
		printf("string:%s\n", buf);
		mciSendString(buf, nullptr, 0, nullptr);
	}
	mciSendString("play BGM", nullptr, 0, nullptr);
}

void RunGameFromOsuFile(Osu* osuFilePointer)
{

	//ReadOsuFile(osuFilePointer, "./beatmapset/1525731/Liella! - START!! True dreams (TV Size) (Sotarks) [Akitoshi's Normal].osu");
	/*                                 X = hit / collect
					p r e e m p t             ↓
	   ├───────────────────────┬──────────────┤
	   0 %     fade_in          100 % opacity*/

	   // Get Preemt and Fade in Time
	int preempt, fade_in;
	if (osuFilePointer->Difficulty.ApproachRate <= 5.0f)
	{ // in ms
		preempt = (float)1200 + 600 * (5.0f - osuFilePointer->Difficulty.ApproachRate) / 5.0f;
		fade_in = (float)800 + 400 * (5.0f - osuFilePointer->Difficulty.ApproachRate) / 5.0f;
	}
	else // AR > 5
	{ // in ms
		preempt = (float)1200 - 750 * (osuFilePointer->Difficulty.ApproachRate - 5.0f) / 5.0f;
		fade_in = (float)800 - 500 * (osuFilePointer->Difficulty.ApproachRate - 5.0f) / 5.0f;
	}

	// Get Judgement Data
	/*AR up to 5	1800 - (AR x 120)
	AR above 5	1200 - ((AR - 5) x 150)
		osu!300 window	79 - (OD x 6) + 0, 5
		osu!100 window	139 - (OD x 8) + 0, 5
		osu!50 window	199 - (OD x 10) + 0, 5*/
	int window300 = 79.0f - (osuFilePointer->Difficulty.OverallDifficulty * 6.0f) + 0.5f;
	int window100 = 139.0f - (osuFilePointer->Difficulty.OverallDifficulty * 8.0f) + 0.5f;
	int window50 = 199.0f - (osuFilePointer->Difficulty.OverallDifficulty * 10.0f) + 0.5f;
	window300 *= 2;
	window100 *= 2;
	//window50 *= 2;
	printf("window: %d|%d|%d\n", window300, window100, window50);

	// Used for Graphics Library
	IMAGE bg;
	ExMessage m;		// 定义消息变量
	int firstHitObjectInQueue = 0, oldFHOIQ = -1;
	int lastHitObjectInQueue = 0, oldLHOIQ = -1;

	// Timing Point
	int nowTimingPoint = 0;
	float nowBeatLength = osuFilePointer->TimingPoints[0].beatLength;
	float nowBeatLengthPercentage = 100.0f;

	int mouseX = 640 / 2;
	int mouseY = 480 / 2;
	int n300 = 0;
	int n100 = 0;
	int n50 = 0;
	int nMiss = 0;
	int nowCombo = 0;
	int nowScore = 0;

	char PATH[256];
	strcpy(PATH, ".\\beatmapsets\\");
	sprintf(PATH + strlen(PATH), "%d\\", osuFilePointer->Metadata.BeatmapSetID);
	char tmpPath[256];
	strcpy(tmpPath, PATH);
	strcat(tmpPath, osuFilePointer->Events.BackgroundFilename);
	loadimage(&bg, tmpPath, 640, 480, true);
	strcpy(tmpPath, PATH);
	strcat(tmpPath, osuFilePointer->General.AudioFilename);
	PlayMusic(tmpPath, 0);

	DWORD startTime = timeGetTime();
	int lastRefresh = startTime;

	while (1)
	{
		//printf("Time since Start: %u\n", nowTime);

		// Check if there's a new TimingPoint
		if (nowTimingPoint < osuFilePointer->TimingPointsCnt - 1 && osuFilePointer->TimingPoints[nowTimingPoint + 1].time > NOWTIME)
		{
			++nowTimingPoint;
			if (osuFilePointer->TimingPoints[nowTimingPoint].uninherited == TRUE)
			{
				nowBeatLength = osuFilePointer->TimingPoints[nowTimingPoint].beatLength;
			}
			else
			{
				nowBeatLengthPercentage = -100.0f / osuFilePointer->TimingPoints[nowTimingPoint].beatLength;
			}
		}

		// Add New Object to Display Queue
		if (lastHitObjectInQueue < osuFilePointer->HitObjectsCnt && osuFilePointer->HitObjects[lastHitObjectInQueue].time - preempt < NOWTIME)
		{
			//printf("Time since Start: %u | ", NOWTIME);
			//printf("printing circle: %d\n", lastHitObjectInQueue);
			++lastHitObjectInQueue;
		}

		// Remove Old Object from Display Queue
		/*if (firstHitObjectInQueue <= lastHitObjectInQueue && osuFilePointer->HitObjects[firstHitObjectInQueue].time + preempt < NOWTIME)
		{
			//printf("Time since Start: %u | ", NOWTIME);
			//printf("clearing circle: %d\n", firstHitObjectInQueue);
			++firstHitObjectInQueue;
		}*/

		// Refresh Screen
		if (NOWTIME - lastRefresh > REFRESHRATE || lastHitObjectInQueue != oldLHOIQ || firstHitObjectInQueue != oldFHOIQ)
		{
			lastRefresh = NOWTIME;
			BeginBatchDraw();

			oldLHOIQ = lastHitObjectInQueue, oldFHOIQ = firstHitObjectInQueue;

			solidrectangle(0, 0, 640, 480);
			putpicture(0, 0, &bg, RGB(85, 85, 85), 150);

			for (int i = lastHitObjectInQueue - 1; i >= firstHitObjectInQueue; --i)
			{
				struct _HITOBJECTS* HitObjectPointer = &osuFilePointer->HitObjects[i];

				// Which Type?
				switch (HitObjectPointer->type & 0b10001011)
				{
				case 0b00000001: //Hit Circle
					//printf("----TYPE: Hit Circle\n");
					printCircle(HitObjectPointer, NOWTIME, preempt);
					break;
				case 0b00000010: //Slider
				{
					//printf("----TYPE: Slider\n");
					switch (HitObjectPointer->objectParams.slider.curveType)
					{
					case 'B':case 'L'://Bazier
					{
						float beatCnt = HitObjectPointer->objectParams.slider.length /
							(osuFilePointer->Difficulty.SliderMultiplier * 100);
						int timeSpan = beatCnt * nowBeatLength * nowBeatLengthPercentage;
						//printf("Now Drwaing Timeframe:%d\n", HitObjectPointer->time);
						printBazier(HitObjectPointer, NOWTIME, preempt, timeSpan);
						break;
					}
					case 'P'://Perfect Arc
					{
						float beatCnt = HitObjectPointer->objectParams.slider.length /
							(osuFilePointer->Difficulty.SliderMultiplier * 100);
						int timeSpan = beatCnt * nowBeatLength * nowBeatLengthPercentage;
						printArc(HitObjectPointer, NOWTIME, preempt, timeSpan);
						break;
					}
					default:
						break;
					}
					break;
				}
				case 0b00001000:
					//printf("----TYPE: Spinner\n");
					break;
				case 0b10000000:
					//printf("----TYPE: Hold\n");
					break;
				default:
					//printf("----TYPE: Unidentified\n");
					break;
				}

			}

			DrawCurser(mouseX, mouseY);

			printInfo((300.0f * n300 + 100.0f * n100 + 50.0f * n50) / (300.0f * (n300 + n100 + n50 + nMiss))
				, nowCombo
				, nowScore);


			EndBatchDraw();
		}

		// Get Miss
		// Which Type?
		int comboMultiplier = nowCombo - 1 > 0 ? nowCombo - 1 : 0;
		switch (osuFilePointer->HitObjects[firstHitObjectInQueue].type & 0b10001011)
		{
			//Score = Hit Value + (Hit Value * ((Combo multiplier * Difficulty multiplier * Mod multiplier) / 25))
			//Combo multiplier	(Combo before this hit - 1) or 0; whichever is higher
			//Stars = Round((HP Drain + Circle Size + Overall Difficulty + Clamp(Hit object count / Drain time in seconds * 8, 0, 16)) / 38 * 5)
		case 0b00000001: //Hit Circle
			if (firstHitObjectInQueue < lastHitObjectInQueue && NOWTIME - osuFilePointer->HitObjects[firstHitObjectInQueue].time > window50)
			{
				switch (osuFilePointer->HitObjects[firstHitObjectInQueue]._HIT_STATUS)
				{
				case 3:
					printf("perfect\n");
					++n300;
					nowScore += 300 + (300 * comboMultiplier / 25);
					++nowCombo;
					break;
				case 2:
					printf("great\n");
					++n100;
					nowScore += 300 + (300 * comboMultiplier / 25);
					++nowCombo;
					break;
				case 1:
					printf("ok\n");
					++n50;
					nowScore += 300 + (300 * comboMultiplier / 25);
					++nowCombo;
					break;
				case 0:
					printf("miss\n");
					++nMiss;
					nowCombo = 0;
					break;
				}
				++firstHitObjectInQueue;
			}
			break;
		case 0b00000010: //Slider
		{
			//printf("----TYPE: Slider\n");
			switch (osuFilePointer->HitObjects[firstHitObjectInQueue].objectParams.slider.curveType)
			{
			case 'B':case 'P':case 'L'://Bazier
			{
				float beatCnt = osuFilePointer->HitObjects[firstHitObjectInQueue].objectParams.slider.length /
					(osuFilePointer->Difficulty.SliderMultiplier * 100);
				int timeSpan = beatCnt * nowBeatLength * nowBeatLengthPercentage;
				int timeStay = timeSpan * osuFilePointer->HitObjects[firstHitObjectInQueue].objectParams.slider.slides;
				if (firstHitObjectInQueue < lastHitObjectInQueue && NOWTIME - (osuFilePointer->HitObjects[firstHitObjectInQueue].time + timeStay) > window50)
				{
					switch (osuFilePointer->HitObjects[firstHitObjectInQueue]._HIT_STATUS)
					{
					case 3:
						printf("perfect\n");
						++n300;
						nowScore += 300 + (300 * comboMultiplier / 25);
						++nowCombo;
						break;
					case 2:
						printf("great\n");
						++n100;
						nowScore += 300 + (300 * comboMultiplier / 25);
						++nowCombo;
						break;
					case 1:
						printf("ok\n");
						++n50;
						nowScore += 300 + (300 * comboMultiplier / 25);
						++nowCombo;
						break;
					case 0:
						printf("miss\n");
						++nMiss;
						nowCombo = 0;
						break;
					}
					++firstHitObjectInQueue;
				}
				break;
			}
			/*case 'P'://Perfect Arc
				break;*/
			default:
				break;
			}
			break;
		}
		case 0b00001000:
			//printf("----TYPE: Spinner\n");
			printf("perfect\n");
			++n300;
			nowScore += 300 + (300 * comboMultiplier / 25);
			++nowCombo;
			++firstHitObjectInQueue;
			break;
		case 0b10000000:
			//printf("----TYPE: Hold\n");
			break;
		default:
			//printf("----TYPE: Unidentified\n");
			break;
		}

		// Events Processing
		// 获取一条鼠标或按键消息

		int readKey = 1;

		if (peekmessage(&m, EM_MOUSE | EM_KEY))
		{
			//m = getmessage(EM_MOUSE | EM_KEY);

			int msgTime = NOWTIME;

			switch (m.message)
			{
			case WM_MOUSEMOVE:
				// 鼠标移动的时候更新光标位置
				mouseX = m.x;
				mouseY = m.y;
				break;

			case WM_LBUTTONDOWN:

				// 如果点左键的同时按下了 Ctrl 键
				if (m.ctrl)
					// 画一个大方块
					rectangle(m.x - 10, m.y - 10, m.x + 10, m.y + 10);
				else
					// 画一个小方块
					rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
				break;

			case WM_KEYDOWN:
				if (readKey)
				{
					switch (m.vkcode)
					{
					case VK_ESCAPE:
						StopMusic();
						return;// 按 ESC 键退出程序
					case 'X':case 'Z':
					{
						for (int i = firstHitObjectInQueue; i < lastHitObjectInQueue; ++i)
						{
							struct _HITOBJECTS* HitObjectPointer = &osuFilePointer->HitObjects[i];
							printf("Key Press Detected, time:%d ,firstHitObjectInQueue time:%d\n", msgTime, HitObjectPointer->time);
							printf("Mouse X:%d, Mouse Y:%d\n", mouseX - CIRCLERADIUS, mouseY - CIRCLERADIUS);
							printf("Target X:%d, Target Y:%d\n", HitObjectPointer->x, HitObjectPointer->y);
							// Judge !
							POINT mouse, target;
							mouse.x = mouseX; mouse.y = mouseY;
							target.x = HitObjectPointer->x + CIRCLERADIUS;
							target.y = HitObjectPointer->y + CIRCLERADIUS;
							if (Dist(mouse, target) <= CIRCLERADIUS)
							{
								// Which Type?
								switch (HitObjectPointer->type & 0b10001011)
								{
								case 0b00000010: //Slider
								case 0b00000001: //Hit Circle
									if (abs(HitObjectPointer->time - msgTime) < window300 && 3 > HitObjectPointer->_HIT_STATUS)
										HitObjectPointer->_HIT_STATUS = 3;
									else if (abs(HitObjectPointer->time - msgTime) < window100 && 2 > HitObjectPointer->_HIT_STATUS)
										HitObjectPointer->_HIT_STATUS = 2;
									else if (abs(HitObjectPointer->time - msgTime) < window50 && 1 > HitObjectPointer->_HIT_STATUS)
										HitObjectPointer->_HIT_STATUS = 1;
									break;
								case 0b00001000:
									//printf("----TYPE: Spinner\n");
									break;
								case 0b10000000:
									//printf("----TYPE: Hold\n");
									break;
								default:
									//printf("----TYPE: Unidentified\n");
									break;
								}
							}
						}
						break;
					}
					default:
						break;
					}
					readKey = 0;
				}
				break;

			case WM_KEYUP:
				readKey = 1;
				break;
			}
		}

		// Check if Song is Over
		if (osuFilePointer->HitObjects[osuFilePointer->HitObjectsCnt - 1].time + 1000 < NOWTIME)
			break;

	}

	// 关闭图形窗口
	//closegraph();
}