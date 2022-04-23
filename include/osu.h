#pragma once

#include "global.h"
#include "static_hash.h"

#define READ_STRING(_OSUSTRUCT_PTR,_BLOCK,_KEY,_VALUE)\
case HASH_S64(#_KEY):\
{\
    char *c=strrchr(##_VALUE##,'\n');\
    if(c)*c='\0';\
    strcpy(##_OSUSTRUCT_PTR##->##_BLOCK##.##_KEY##,##_VALUE##);\
    printf("----Found Key: "#_KEY);\
    printf(" | Value:%s",##_VALUE##);\
    break;\
}

#define READ_INT(_OSUSTRUCT_PTR,_BLOCK,_KEY,_VALUE)\
case HASH_S64(#_KEY):\
{\
    int val; \
    sscanf(##_VALUE##, "%d", &val); \
    _OSUSTRUCT_PTR##->##_BLOCK##.##_KEY## = val; \
    printf("----Found Key: "#_KEY); \
    printf(" | Value:%d\n", _OSUSTRUCT_PTR##->##_BLOCK##.##_KEY##); \
    break; \
}

#define READ_FLOAT(_OSUSTRUCT_PTR,_BLOCK,_KEY,_VALUE)\
case HASH_S64(#_KEY):\
{\
    float val;\
    sscanf(##_VALUE##,"%f",&val);\
    _OSUSTRUCT_PTR##->##_BLOCK##.##_KEY##=val;\
    printf("----Found Key: "#_KEY);\
    printf(" | Value:%f\n",_OSUSTRUCT_PTR##->##_BLOCK##.##_KEY##);\
    break;\
}

#define READ_AND_FORWARD(_SOURCE_STRING,_FORMAT,_DEST,_FOLLOWING_CHAR)\
{\
    sscanf(##_SOURCE_STRING##,##_FORMAT##,##_DEST##);\
    _SOURCE_STRING##=(char*)strchr(##_SOURCE_STRING##,##_FOLLOWING_CHAR##)?strchr(##_SOURCE_STRING##,##_FOLLOWING_CHAR##)+1:NULL;\
}

struct _GENERAL                                 //��������
{
    char  AudioFilename[max_length_short];      //��Ƶ�ļ�����ڵ�ǰ�ļ��е�λ��
    int   AudioLeadIn = 0;                      //��Ƶ����ǰ������ʱ�䣨�Ժ���Ϊ��λ��
    int   PreviewTime = -1;                     //��ƵԤ���㿪ʼ��ʱ�䣨�Ժ���Ϊ��λ��
    int   Countdown = 1;                        //��һ���������֮ǰ�ĵ����ٶ� (0 = �޵����� 1 = ����, 2 = �ٶȼ���, 3 = �����ٶ�)
    char  SampleSet[8] = "Normal";              //���ʱ���δָ��������ʱʹ�õ�Ĭ��ֵ (Normal, Soft, Drum)
    float StackLeniency = 0.7f;                 //�ѵ��ص������ʱ����ֵ���� (0�C1)	
    int   Mode = 0;                             //��Ϸģʽ (0 = osu!, 1 = osu!taiko, 2 = osu!catch, 3 = osu!mania)
    bool  LetterboxInBreaks = FALSE;            //��Ϣʱ���Ƿ��ú�ɫ�߿򽫱���ͼ��Χ����
    bool  UseSkinSprites = FALSE;               //���°��Ƿ����ʹ����ҵ�Ƥ��ͼƬ
    char  OverlayPosition[16] = "NoChange";     //������Ȧ��������ֵ�λ�� (NoChange = ʹ��Ƥ�����ã� Below = draw ���Ȧ�����������, Above = ���Ȧ�����������)
    char  SkinPreference[max_length_short];     //������ʱ�Ƽ�ʹ�õ�Ƥ��
    bool  EpilepsyWarning = FALSE;              //�Ƿ������濪ʼǰ��ʾ Storyboard �������˲��ʵ���˸
    int   CountdownOffset = 0;                  //�����ڵ�һ�����ǰ������
    bool  SpecialStyle = FALSE;                 //��osu!maniaģʽ���Ƿ�ʹ�� "N+1" ���
    bool  WidescreenStoryboard = FALSE;         //���°��Ƿ�֧�ֿ�����ʾ	
    bool  SamplesMatchPlaybackRate = FALSE;     //������ʱʹ���ٶȸ��ĵ�Modʱ�Ƿ������Ч���ٶ�	
};

struct _EDITOR                                  //�༭������
{
    //int Bookmarks;                            //��ǩ(�Ժ���Ϊ��λ)
    float DistanceSpacing;                      //�����Ƶı���
    float BeatDivisor;                          //����ϸ��
    int   GridSize;                             //����ߴ�
    float TimelineZoom;                         //���ʱ���������
};

struct _METADATA                                //����Ԫ����
{
    char Title[max_length_medium];              //������������
    char TitleUnicode[max_length_medium];       //��������
    char Artist[max_length_medium];             //��������������
    char ArtistUnicode[max_length_medium];      //����������
    char Creator[max_length_medium];            //���洴����
    char Version[max_length_medium];            //�Ѷ�����
    char Source[max_length_medium];             //������ԭʼý��
    char Tags[max_length_long];                 //������
    int  BeatmapID;                             //����ID
    int  BeatmapSetID;                          //���漯ID
};

struct _DIFFICULTY
{
    float HPDrainRate;                          //HP ���� (0�C10)
    float CircleSize;                           //CS ���� (0�C10)
    float OverallDifficulty;                    //OD ���� (0�C10)
    float ApproachRate;                         //AR ���� (0�C10)
    float SliderMultiplier;                     //��100osu! ����Ϊ��λ��ÿ�Ļ����������ʣ�������Ϊ1��һ�Ļ���Ϊ 100 osu! ���س���
    float SliderTickRate;                       //ÿ���л����������
};

struct _EVENTS
{
    char  BackgroundFilename[max_length_short]; //����ͼƬ�ļ���
    int   xOffset;                              //����ͼƬX����ƫ��
    int   yOffset;                              //����ͼƬY����ƫ��
};

struct _TIMINGPOINTS
{
    int   time;                                 //ʱ������Ŀ�ʼʱ�䣬 ��������Ƶ��ʼ�Ժ���Ϊ��λ��ʱ��
    float beatLength;                           //���������������壺�ڷǼ̳�ʱ��㣨���ߣ��У�Ϊ1�ĵĳ���ʱ�䣬��λΪ���롣 �ڼ̳�ʱ��㣨���ߣ��У�һ���ٷֱȵ�λ���渺����������������
    int   meter;                                //һ���е�С������
    int   sampleSet;                            //���ʹ�õĲ����� (0 = ����Ĭ�����ã� 1 = normal, 2 = soft, 3 = drum).
    int   sampleIndex;                          //���ʹ�õĲ����������� 0 ��ʾ osu! Ĭ����Ч��
    int   volume;                               //����������ٷֱ�
    bool  uninherited;                          //��ʾ���ʱ����Ƿ�Ϊ�Ǽ̳�ʱ���
    int   effects;                              //ʹʱ��������ж���Ч����λ��
};

struct _COLOURS
{
    int Combo[16][3];
    int SliderTrackOverride[3];
    int SliderBorder[3];

    int ComboCnt;
    bool SliderTrackOverrideFlag;
    bool SliderBorderFlag;
    /*ѡ��	����
    Combo#��# Ϊ����	���ӵ�������ɫ
    SliderTrackOverride	���ӵĻ��������ɫ
    SliderBorder	����߿���ɫ*/
};

struct _HITOBJECTS
{
    int x, y;                                   //�� osu! ����Ϊ��λ�����λ��
    int time;                                   //���Ӧ�ñ����е�ʱ�䣬��������Ƶ��ʼ�Ժ���Ϊ��λ��ʱ��
    int type;                                   //��λ����ʾ���������
    /*Type��������ͣ�
    ���ж���������8λ�����洢������ÿ��λ����������ĺ��塣���������������0��1��3�� ��7 ������͵���ߣ���ɣ�

    0�����Ȧ
    1������
    3��ת��
    7��osu!mania����
    ʣ���λ���������µ���������ѡ�������������ɫ��

    2��������
    4�C6: һ��3λ������ָ���������Ϊ�µ�����ʱҪ������������ɫ����*/
    int hitSound;                               //��λ����ʾ�����Ӧ�õĴ����Ч
    union __OBJECT_PARAMS
    {
        // HIT CIRCLE HAS NO OBJECTPARAMS
        struct _SLIDER
        {
            //              curveType | curvePoints                                     , slides, length          ,edgeSounds, edgeSets             ,hitSample
            //184, 226, 1626, 6, 0, B | 140:230 | 107 : 245 | 107 : 245 | 83 : 234 | 69 : 205, 2, 134.999995880127, 6 | 8 | 4, 1 : 2 | 0 : 3 | 0 : 3, 0 : 0 : 0 : 0 :
            char curveType;
            int curvePoints[32][2];
            int slides;
            float length;
            int edgeSounds[4];
            int edgeSets[4][2];

            int curvePointsCnt;
        }slider;
        /***curveType ����ĸ����**�˻������������ (B = ��������, C = ���� Catmull-Rom ����, L = ����, P = ����Բ��)
curvePoints ����|�ָ����ַ����б��� ���ڹ��컬����ê�㡣ÿ����ĸ�ʽΪ x:y.
slides ���������� ��ұ����ڻ������֮ǰ���ظ��滬�����ߵĴ�����Ҳ���Խ������Ϊ����������1��
length ��С������ �����Ŀɼ����ȣ���λΪosu! ���أ�
**edgeSounds ����|�ָ��������б���**�û�������ʱ��������Ч����һ�������ǵ�һ�ε�������ʱ���ŵ����������һ�������ǵ��ﻬ��ĩ�˲��ŵ�������
edgeSets ����|�ָ����ַ����б��� ����edgeSounds�Ĳ�������ÿ���������ĸ�ʽΪnormalSet:additionSet���� �����Ч������һ����˼.
Slider curves�������������ͣ�
��Ϊ������������ʱ��x��y���ڵ�һ�����Ƶ㣬ʣ�µ�ΪcurvePoints��

osu!�����ֻ�����������:

�������� ��B���� ��������ʹ�ñ������� ��ͨ���ص����ǵĽ��㣬���Խ�������������ߺϲ������������С�
���� Catmull-Rom ���� ��C���� ���� Catmull-Rom ���ߣ�Ӣ��ά��ҳ�棩 �Ǳ��������ߵĲ�ֵ�������������ȱ���Ӿ����������������ں��ٱ�ʹ�á�
���ԣ�L���� ��Щ���������е�֮���γ�һ��ֱ��·����
����Բ�� (P): ����Բ�ν��������㣨������������λ�ã����޶�Բ�ε���״��ʹ���������ϵĵ�Ὣ�������������л�Ϊ�������ߡ�
��������ĳ��ȣ�Length�����ڶ����������͵ĳ��ȣ��������������ֱ������Ŀ�곤�ȣ�

���ڱ��ȡ�Catmull-Rom���������ߣ��������ߵ�ĩβ��ֱ����ʽ���졣
��������Բ�����ߣ�����Բ����ʽ���졣
ע�⣺�����ĳ��ȣ�Length��������ȷ����ɻ��������ʱ�䡣 length / (SliderMultiplier*100)*beatLength ����������ɻ�����Ҫ���ٺ��� ������ beatLength �Ѹ��ݼ̳�ʱ�����е�������*/
        struct _SPINNER
        {
            int endTime; // endTime ��������: ת�̵Ľ���ʱ�䣬��������Ƶ��ʼ�Ժ���Ϊ��λ��ʱ��
        }spinner;
        struct _HOLD
        {
            //x, y, time, type, hitSound, endTime:hitSample
            int endTime;
        }hold;
    }objectParams;
    
    struct __HIT_SAMPLE
    {
        /* normalSet ���������� ��ͨ��Ч�Ĳ�������
           additionSet ���������� whistle��finish��clap�Ĳ�������
            index ���������� �������������������Ϊ0����ʹ��ʱ��������õĲ�����������
            volume ���������� ��������������1��100�����������Ϊ0����ʹ��ʱ��������õ�������
            filename ���ַ������� �Զ����ⲿ��Ч�ļ�����*/

            /*normalSet �� additionSet ������������κ�һ����
            0�����Զ��������
            ������ͨ�����Ч����������ʱ��������������
            ���ڸ��Ӵ����Ч�� ����������ͨ�����Ч������������
            1��Normal ����
            2��Soft ����
            3��Drum ����
            ������Щѡ��������⣩������ȷ��������ƥ�����������ĸ������ļ����ļ���Ϊ <sampleSet>-hit<hitSound><index>.wav�����У�

            sampleSet Ϊ normal��soft���� drum����normalSet ���� additionSet ��������һ���������ŵ���Ч
            hitSound Ϊ normal ��whistle��finish������clap
            index ������� index һ���������0��1����Ժ��ԡ�
            �����µ�һ����ƥ���ļ�����Ŀ¼�м��������ļ���

            �����ļ��У����index����0
            Ƥ�������index���Ƴ�
            osu!Ĭ����Դ�����index���Ƴ�
            ����д��filename�����Ქ�Ÿ���������ȡ����֮���������ļ��е�����ļ�*/
        int normalSet;
        int additionalSet;
        int index;
        int volume;
        char filename[max_length_short];
    }hitSample;           //�й��ڻ��ж���ʱ������Щ��������Ϣ�� ����hitSound�������

    // Self Added: Combo(��ʾ����ʲô����), COLOUR(��ʾ����ʲô��ɫ)
    char _COMBO;
    int _COLOUR_COMBO_ID;
    int _COLOUR[3];
    int _HIT_STATUS = 0; // 0:Miss, 1:50, 2:100, 3:300
};

typedef struct
{
    struct _GENERAL General;
    struct _EDITOR Editor;
    struct _METADATA Metadata;
    struct _DIFFICULTY Difficulty;
    struct _EVENTS Events;
    struct _TIMINGPOINTS TimingPoints[256];
    struct _COLOURS Colours;
    struct _HITOBJECTS HitObjects[1024];
    int TimingPointsCnt;
    int HitObjectsCnt;
}Osu;

enum BLOCK{GENERAL,EDITOR,METADATA,DIFFICULTY,EVENTS,TIMINGPOINTS,COLOURS,HITOBJECTS};

int ProcessLine(Osu* OsuStruct, const char* str, enum BLOCK Block);
void ReadOsuFile(Osu* osuFilePointer, const char* PATH, int debug = 0);