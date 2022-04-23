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

struct _GENERAL                                 //基本数据
{
    char  AudioFilename[max_length_short];      //音频文件相对于当前文件夹的位置
    int   AudioLeadIn = 0;                      //音频播放前静音的时间（以毫秒为单位）
    int   PreviewTime = -1;                     //音频预览点开始的时间（以毫秒为单位）
    int   Countdown = 1;                        //第一个物件出现之前的倒数速度 (0 = 无倒数， 1 = 正常, 2 = 速度减半, 3 = 两倍速度)
    char  SampleSet[8] = "Normal";              //如果时间点未指定采样集时使用的默认值 (Normal, Soft, Drum)
    float StackLeniency = 0.7f;                 //堆叠重叠物件的时间阈值乘数 (0–1)	
    int   Mode = 0;                             //游戏模式 (0 = osu!, 1 = osu!taiko, 2 = osu!catch, 3 = osu!mania)
    bool  LetterboxInBreaks = FALSE;            //休息时间是否用黑色边框将背景图像围起来
    bool  UseSkinSprites = FALSE;               //故事板是否可以使用玩家的皮肤图片
    char  OverlayPosition[16] = "NoChange";     //打击物件圈和物件数字的位置 (NoChange = 使用皮肤设置， Below = draw 打击圈在物件数字下, Above = 打击圈在物件数字上)
    char  SkinPreference[max_length_short];     //在游玩时推荐使用的皮肤
    bool  EpilepsyWarning = FALSE;              //是否在谱面开始前提示 Storyboard 会有令人不适的闪烁
    int   CountdownOffset = 0;                  //倒数在第一个物件前的拍数
    bool  SpecialStyle = FALSE;                 //在osu!mania模式中是否使用 "N+1" 风格
    bool  WidescreenStoryboard = FALSE;         //故事板是否支持宽屏显示	
    bool  SamplesMatchPlaybackRate = FALSE;     //当游玩时使用速度更改的Mod时是否更改音效的速度	
};

struct _EDITOR                                  //编辑器设置
{
    //int Bookmarks;                            //书签(以毫秒为单位)
    float DistanceSpacing;                      //间距控制的倍数
    float BeatDivisor;                          //节拍细分
    int   GridSize;                             //网格尺寸
    float TimelineZoom;                         //物件时间轴的缩放
};

struct _METADATA                                //谱面元数据
{
    char Title[max_length_medium];              //罗马化歌曲标题
    char TitleUnicode[max_length_medium];       //歌曲标题
    char Artist[max_length_medium];             //罗马化歌曲艺术家
    char ArtistUnicode[max_length_medium];      //歌曲艺术家
    char Creator[max_length_medium];            //谱面创建者
    char Version[max_length_medium];            //难度名称
    char Source[max_length_medium];             //歌曲的原始媒体
    char Tags[max_length_long];                 //搜索词
    int  BeatmapID;                             //谱面ID
    int  BeatmapSetID;                          //谱面集ID
};

struct _DIFFICULTY
{
    float HPDrainRate;                          //HP 设置 (0–10)
    float CircleSize;                           //CS 设置 (0–10)
    float OverallDifficulty;                    //OD 设置 (0–10)
    float ApproachRate;                         //AR 设置 (0–10)
    float SliderMultiplier;                     //以100osu! 像素为单位的每拍基础滑条速率（若设置为1则一拍滑条为 100 osu! 像素长）
    float SliderTickRate;                       //每拍中滑条点的数量
};

struct _EVENTS
{
    char  BackgroundFilename[max_length_short]; //背景图片文件名
    int   xOffset;                              //背景图片X坐标偏移
    int   yOffset;                              //背景图片Y坐标偏移
};

struct _TIMINGPOINTS
{
    int   time;                                 //时间区间的开始时间， 从谱面音频开始以毫秒为单位的时间
    float beatLength;                           //此属性有两个含义：在非继承时间点（红线）中，为1拍的持续时间，单位为毫秒。 在继承时间点（绿线）中，一个百分比单位的逆负滑条速率增倍器。
    int   meter;                                //一拍中的小节数量
    int   sampleSet;                            //物件使用的采样集 (0 = 谱面默认设置， 1 = normal, 2 = soft, 3 = drum).
    int   sampleIndex;                          //物件使用的采样集索引。 0 表示 osu! 默认音效。
    int   volume;                               //物件的音量百分比
    bool  uninherited;                          //表示这个时间点是否为非继承时间点
    int   effects;                              //使时间区间具有额外效果的位符
};

struct _COLOURS
{
    int Combo[16][3];
    int SliderTrackOverride[3];
    int SliderBorder[3];

    int ComboCnt;
    bool SliderTrackOverrideFlag;
    bool SliderBorderFlag;
    /*选项	描述
    Combo#，# 为整数	附加的连击颜色
    SliderTrackOverride	附加的滑条轨道颜色
    SliderBorder	滑块边框颜色*/
};

struct _HITOBJECTS
{
    int x, y;                                   //以 osu! 像素为单位的物件位置
    int time;                                   //物件应该被击中的时间，从谱面音频开始以毫秒为单位的时间
    int type;                                   //用位符表示的物件类型
    /*Type（物件类型）
    命中对象类型以8位整数存储，其中每个位都具有特殊的含义。基础的物件类型由0，1，3， 和7 （从最低到最高）组成：

    0：打击圈
    1：滑条
    3：转盘
    7：osu!mania滑条
    剩余的位用于区分新的连击并有选择地跳过连击颜色：

    2：新连击
    4–6: 一个3位整数，指定此物件成为新的连击时要跳过的连击颜色数。*/
    int hitSound;                               //用位符表示物件所应用的打击音效
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
        /***curveType （字母）：**此滑块的曲线类型 (B = 贝兹曲线, C = 向心 Catmull-Rom 曲线, L = 线性, P = 完美圆弧)
curvePoints （用|分隔的字符串列表）： 用于构造滑条的锚点。每个点的格式为 x:y.
slides （整数）： 玩家必须在滑条完成之前来回跟随滑条曲线的次数。也可以将其解释为往返计数加1。
length （小数）： 滑条的可见长度（单位为osu! 像素）
**edgeSounds （用|分隔的整数列表）：**敲击滑条边时发出的音效。第一个声音是第一次单击滑块时播放的声音，最后一个声音是到达滑块末端播放的声音。
edgeSets （用|分隔的字符串列表）： 用于edgeSounds的采样集。每个采样集的格式为normalSet:additionSet，与 打击音效部分是一个意思.
Slider curves（滑条曲线类型）
当为滑条构建曲线时，x和y用于第一个控制点，剩下的为curvePoints。

osu!有四种滑条曲线类型:

贝兹曲线 （B）： 可以任意使用贝兹曲线 。通过重叠它们的交点，可以将多个贝塞尔曲线合并到单个滑块中。
向心 Catmull-Rom 曲线 （C）： 向心 Catmull-Rom 曲线（英文维基页面） 是贝塞尔曲线的插值替代方法。由于缺乏视觉吸引力，它们现在很少被使用。
线性（L）： 这些曲线在所有点之间形成一条直线路径。
完美圆形 (P): 完美圆形仅用三个点（包括打击物件的位置）来限定圆形的形状。使用三个以上的点会将滑条曲线类型切换为贝兹曲线。
如果滑条的长度（Length）长于定义曲线类型的长度，滑条会继续延伸直到到达目标长度：

对于贝兹、Catmull-Rom、线性曲线，会在曲线的末尾以直线形式延伸。
对于完美圆形曲线，会以圆弧方式延伸。
注意：滑条的长度（Length）可用于确定完成滑条所需的时间。 length / (SliderMultiplier*100)*beatLength 告诉我们完成滑条需要多少毫秒 （假设 beatLength 已根据继承时间点进行调整）。*/
        struct _SPINNER
        {
            int endTime; // endTime （整数）: 转盘的结束时间，从谱面音频开始以毫秒为单位的时间
        }spinner;
        struct _HOLD
        {
            //x, y, time, type, hitSound, endTime:hitSample
            int endTime;
        }hold;
    }objectParams;
    
    struct __HIT_SAMPLE
    {
        /* normalSet （整数）： 普通音效的采样集。
           additionSet （整数）： whistle，finish和clap的采样集。
            index （整数）： 采样集索引，如果设置为0，将使用时间点所设置的采样集索引。
            volume （整数）： 采样集的音量（1到100）。如果设置为0，将使用时间点所设置的音量。
            filename （字符串）： 自定义外部音效文件名。*/

            /*normalSet 和 additionSet 可以是下面的任何一个：
            0：无自定义采样集
            对于普通打击音效，采样集由时间点采样集决定。
            对于附加打击音效， 采样集由普通打击音效采样集决定。
            1：Normal 集合
            2：Soft 集合
            3：Drum 集合
            所有这些选项（音量除外）都用于确定给定的匹配声音播放哪个声音文件。文件名为 <sampleSet>-hit<hitSound><index>.wav，其中：

            sampleSet 为 normal，soft或者 drum，由normalSet 或者 additionSet 其中任意一个决定播放的音效
            hitSound 为 normal ，whistle，finish，或者clap
            index 与上面的 index 一样，如果是0和1则可以忽略。
            从以下第一个含匹配文件名的目录中加载声音文件：

            谱面文件夹，如果index不是0
            皮肤，如果index被移除
            osu!默认资源，如果index被移除
            当填写了filename，不会播放附加声音，取而代之播放谱面文件夹的这个文件*/
        int normalSet;
        int additionalSet;
        int index;
        int volume;
        char filename[max_length_short];
    }hitSample;           //有关在击中对象时播放哪些样本的信息。 它与hitSound密切相关

    // Self Added: Combo(显示的是什么数字), COLOUR(显示的是什么颜色)
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