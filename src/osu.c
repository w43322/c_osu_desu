#include "osu.h"

int ProcessLine(Osu* OsuStruct, const char* str, enum BLOCK Block)
{
    size_t len = strlen(str);

    // Debug
    printf("Block: %d | Input String: %s", Block, str);

    // Get Key and Value
    char* colon = (char*)strchr(str, ':'); // Find Location of ':'
    if (colon && Block != 7 && Block != 6) // If there is a ':'
    {
        char* value = colon + 1; // Value is behind colon
        if (*value == ' ') ++value; // Might be white space

        *colon = '\0'; // Change ':' to '\0'
        char* key = (char*)str;

        switch (Block)
        {
        case GENERAL:
            switch (hash_string(key))
            {
                READ_STRING(OsuStruct, General, AudioFilename, value)
                    READ_INT(OsuStruct, General, AudioLeadIn, value)
                    READ_INT(OsuStruct, General, PreviewTime, value)
                    READ_INT(OsuStruct, General, Countdown, value)
                    READ_STRING(OsuStruct, General, SampleSet, value)
                    READ_FLOAT(OsuStruct, General, StackLeniency, value)
                    READ_INT(OsuStruct, General, Mode, value)
                    READ_INT(OsuStruct, General, LetterboxInBreaks, value)
                    READ_INT(OsuStruct, General, UseSkinSprites, value)
                    READ_STRING(OsuStruct, General, OverlayPosition, value)
                    READ_STRING(OsuStruct, General, SkinPreference, value)
                    READ_INT(OsuStruct, General, EpilepsyWarning, value)
                    READ_INT(OsuStruct, General, CountdownOffset, value)
                    READ_INT(OsuStruct, General, SpecialStyle, value)
                    READ_INT(OsuStruct, General, WidescreenStoryboard, value)
                    READ_INT(OsuStruct, General, SamplesMatchPlaybackRate, value)
            default:
                printf("----Key not Found: %s\n", key);
                break;
            }break;
        case EDITOR:
            switch (hash_string(key))
            {
                READ_FLOAT(OsuStruct, Editor, DistanceSpacing, value)
                    READ_FLOAT(OsuStruct, Editor, BeatDivisor, value)
                    READ_INT(OsuStruct, Editor, GridSize, value)
                    READ_FLOAT(OsuStruct, Editor, TimelineZoom, value)
            default:
                printf("----Key not Found: %s\n", key);
                break;
            }break;
        case METADATA:
            switch (hash_string(key))
            {
                READ_STRING(OsuStruct, Metadata, Title, value)
                    READ_STRING(OsuStruct, Metadata, TitleUnicode, value)
                    READ_STRING(OsuStruct, Metadata, Artist, value)
                    READ_STRING(OsuStruct, Metadata, ArtistUnicode, value)
                    READ_STRING(OsuStruct, Metadata, Creator, value)
                    READ_STRING(OsuStruct, Metadata, Version, value)
                    READ_STRING(OsuStruct, Metadata, Source, value)
                    READ_STRING(OsuStruct, Metadata, Tags, value)
                    READ_INT(OsuStruct, Metadata, BeatmapID, value)
                    READ_INT(OsuStruct, Metadata, BeatmapSetID, value)
            default:
                printf("----Key not Found: %s\n", key);
                break;
            }break;
        case DIFFICULTY:
            switch (hash_string(key))
            {
                READ_FLOAT(OsuStruct, Difficulty, HPDrainRate, value)
                    READ_FLOAT(OsuStruct, Difficulty, CircleSize, value)
                    READ_FLOAT(OsuStruct, Difficulty, OverallDifficulty, value)
                    READ_FLOAT(OsuStruct, Difficulty, ApproachRate, value)
                    READ_FLOAT(OsuStruct, Difficulty, SliderMultiplier, value)
                    READ_FLOAT(OsuStruct, Difficulty, SliderTickRate, value)
            default:
                printf("----Key not Found: %s\n", key);
                break;
            }break;
        case EVENTS:
            switch (hash_string(key))
            {
                //to be filled by zhb
            default:
                printf("----Key not Found: %s\n", key);
                break;
            }break;
        default:break;
        }
    }
    else
    {
        switch (Block)
        {
        case EVENTS:
        {
            if (str[0] == '0' && str[1] == ',' && str[2] == '0' && str[3] == ',')
            {
                char* remain = (char*)str + 4;
                char* rmost;
                if (*remain == '"')
                {
                    ++remain;
                    rmost = strrchr(remain, '"');
                    *rmost = '\0';
                    strcpy(OsuStruct->Events.BackgroundFilename, remain);
                    //sscanf(remain, "%s", &OsuStruct->Events.BackgroundFilename);
                    ++rmost;
                }
                else
                {
                    sscanf(remain, "%s", &OsuStruct->Events.BackgroundFilename);
                    rmost = strrchr(remain, ',');
                }
                if (rmost)
                    sscanf(rmost, ",%d,%d\n", &OsuStruct->Events.xOffset, &OsuStruct->Events.yOffset);
                else
                {
                    OsuStruct->Events.xOffset = 0;
                    OsuStruct->Events.yOffset = 0;
                }
                printf("----READ: BGFileName: %s, oX:%d, oY:%d\n", OsuStruct->Events.BackgroundFilename, OsuStruct->Events.xOffset, OsuStruct->Events.yOffset);
            }
            break;
        }
        case COLOURS:
        {
            switch (str[0]) // Combo ?
            {
            case 'C':
            {
                int i;
                char* remain = (char*)str + 5;
                READ_AND_FORWARD(remain, "%d", &i, ':');
                --i; ++OsuStruct->Colours.ComboCnt;
                READ_AND_FORWARD(remain, "%d", &OsuStruct->Colours.Combo[i][0], ',')
                    READ_AND_FORWARD(remain, "%d", &OsuStruct->Colours.Combo[i][1], ',')
                    sscanf(remain, "%d", &OsuStruct->Colours.Combo[i][2]);
                printf("----READ: Color[%d]: %d,%d,%d\n", i, OsuStruct->Colours.Combo[i][0], OsuStruct->Colours.Combo[i][1], OsuStruct->Colours.Combo[i][2]);
                break;
            }
            case 'S':
            {
                char* remain = (char*)strchr(str, ':') + 1;
                switch (str[6]) // SliderTrackOverride or SliderBorder ?
                {
                case 'T':
                    READ_AND_FORWARD(remain, "%d", &OsuStruct->Colours.SliderTrackOverride[0], ',')
                        READ_AND_FORWARD(remain, "%d", &OsuStruct->Colours.SliderTrackOverride[1], ',')
                        sscanf(remain, "%d", &OsuStruct->Colours.SliderTrackOverride[2]);
                    break;
                case 'B':
                    READ_AND_FORWARD(remain, "%d", &OsuStruct->Colours.SliderBorder[0], ',')
                        READ_AND_FORWARD(remain, "%d", &OsuStruct->Colours.SliderBorder[1], ',')
                        sscanf(remain, "%d", &OsuStruct->Colours.SliderBorder[2]);
                    break;
                default:
                    break;
                }
            }
            }
            break;
        }
        case TIMINGPOINTS:
        {
            sscanf(str, "%d,%f,%d,%d,%d,%d,%d,%d",
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].time,
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].beatLength,
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].meter,
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].sampleSet,
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].sampleIndex,
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].volume,
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].uninherited,
                &OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].effects);
            printf("----Read: TimingPoints[%d]: %d,%f,%d,%d,%d,%d,%d,%d\n",
                OsuStruct->TimingPointsCnt,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].time,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].beatLength,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].meter,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].sampleSet,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].sampleIndex,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].volume,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].uninherited,
                OsuStruct->TimingPoints[OsuStruct->TimingPointsCnt].effects);
            ++OsuStruct->TimingPointsCnt;
            break;
        }
        case HITOBJECTS:
        {
            char* remain = (char*)str;

            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].x, ',');
            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].y, ',');
            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].time, ',');
            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].type, ',');
            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSound, ',');

            printf("----Read: HitObjects[%d]: x:%d, y:%d, time:%d, type:%d, hitSound:%d\n",
                OsuStruct->HitObjectsCnt,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].x,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].y,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].time,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].type,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSound);

            // Stacking Object Processing
            if (OsuStruct->HitObjectsCnt
                && OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].x == OsuStruct->HitObjects[OsuStruct->HitObjectsCnt - 1].x
                && OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].y == OsuStruct->HitObjects[OsuStruct->HitObjectsCnt - 1].y)
            {
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].x += STACKED_OBJECT_OFFSET;
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].y += STACKED_OBJECT_OFFSET;
            }
            if (OsuStruct->HitObjectsCnt
                && OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].x == OsuStruct->HitObjects[OsuStruct->HitObjectsCnt - 1].x - STACKED_OBJECT_OFFSET
                && OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].y == OsuStruct->HitObjects[OsuStruct->HitObjectsCnt - 1].y - STACKED_OBJECT_OFFSET)
            {
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].x += STACKED_OBJECT_OFFSET * 2;
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].y += STACKED_OBJECT_OFFSET * 2;
            }

            // Combo & Colour Processing
            if (OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].type & 0b100) // Start of a new Combo
            {
                printf("----Starts a new Combo\n");

                // Set Combo (Displayed Number)
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COMBO = 1 + '0';

                // Set Colour
                if (OsuStruct->HitObjectsCnt == 0)
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR_COMBO_ID = 1;
                else
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR_COMBO_ID =
                    (OsuStruct->HitObjects[OsuStruct->HitObjectsCnt - 1]._COLOUR_COMBO_ID + 1) % OsuStruct->Colours.ComboCnt;
            }
            else // Is with last Combo
            {
                // Set Combo (Displayed Number)
                if (OsuStruct->HitObjectsCnt == 0)
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COMBO = 1 + '0';
                else
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COMBO =
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt - 1]._COMBO + 1;

                // Set Colour
                if (OsuStruct->HitObjectsCnt == 0)
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR_COMBO_ID = 1;
                else
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR_COMBO_ID = OsuStruct->HitObjects[OsuStruct->HitObjectsCnt - 1]._COLOUR_COMBO_ID;

            }

            // Set Colour RGB values
            OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR[0] = OsuStruct->Colours.Combo
                [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR_COMBO_ID][0];
            OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR[1] = OsuStruct->Colours.Combo
                [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR_COMBO_ID][1];
            OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR[2] = OsuStruct->Colours.Combo
                [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt]._COLOUR_COMBO_ID][2];

            switch (OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].type & 0b10001011)
            {
            case 0b00000001:
                printf("----TYPE: Hit Circle\n");
                break;
            case 0b00000010:
            {
                printf("----TYPE: Slider\n");
                READ_AND_FORWARD(remain, "%c", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curveType, '|');
                printf("--------Read: curveType:%c\n",
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curveType);

                // Get Curve Points
                char* curvePointsStr = remain - 1;
                remain = (char*)strchr(remain, ',') + 1;
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePointsCnt = 0;
                while (*curvePointsStr != ',')
                {
                    sscanf(curvePointsStr, "|%d:%d"
                        , &(OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePoints
                            [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePointsCnt][0])
                        , &(OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePoints
                            [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePointsCnt][1]));
                    ++OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePointsCnt;
                    ++curvePointsStr;
                    while (*curvePointsStr != '|' && *curvePointsStr != ',')
                        ++curvePointsStr;
                }
                for (int i = 0; i < OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePointsCnt; ++i)
                    printf("------------Read: curvePoints[%d]: %d:%d\n"
                        , i
                        , OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePoints[i][0]
                        , OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.curvePoints[i][1]);

                // Get SLIDES and LENGTH
                READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides, ',')
                    READ_AND_FORWARD(remain, "%f", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.length, ',')
                    printf("--------Read: slides:%d, length:%f\n"
                        , OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides
                        , OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.length);

                if (!remain)break;

                // Get edgeSounds & edgeSets
                for (int i = 0; i < OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides; ++i)
                    READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSounds[i], '|')
                    READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSounds
                        [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides], ',')
                    for (int i = 0; i < OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides; ++i)
                    {
                        READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSets[i][0], ':')
                            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSets[i][1], '|')
                    }
                READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSets
                    [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides][0], ':')
                    READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSets
                        [OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides][1], ',')
                    for (int i = 0; i <= OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides; ++i)
                        printf("------------Read: edgeSounds[%d]: %d\n"
                            , i, OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSounds[i]);
                for (int i = 0; i <= OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.slides; ++i)
                    printf("------------Read: edgeSounds[%d]: x:%d, y:%d\n"
                        , i
                        , OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSets[i][0]
                        , OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.slider.edgeSets[i][1]);

                break;
            }
            case 0b00001000:
                printf("----TYPE: Spinner\n");
                READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.spinner.endTime, ',');
                printf("--------Read: endTime:%d\n",
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.spinner.endTime);
                break;
            case 0b10000000:
                printf("----TYPE: Hold\n");
                READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.hold.endTime, ':');
                printf("--------Read: endTime:%d\n",
                    OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].objectParams.hold.endTime);
                break;
            default:
                printf("----TYPE: Unidentified\n");
                break;
            }

            if (!remain) break;

            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.normalSet, ':');
            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.additionalSet, ':');
            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.index, ':');
            READ_AND_FORWARD(remain, "%d", &OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.volume, ':');
            READ_AND_FORWARD(remain, "%s", OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.filename, '\n');

            if (!remain) break;

            printf("--------Read: hitSample: x:%d, y:%d, index:%d, volume:%d, filename:",
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.normalSet,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.additionalSet,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.index,
                OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.volume);
            if (strlen(remain))printf("%s", OsuStruct->HitObjects[OsuStruct->HitObjectsCnt].hitSample.filename);
            printf("\n");

            if (strlen(remain))printf("----ERROR-------Remain:%s", remain);


            ++OsuStruct->HitObjectsCnt;
            break;
        }
        default:
            break;
        }
    }
    return 0;
}

void ReadOsuFile(Osu* osuFilePointer, const char* PATH, int debug)
{


    printf("-------------\nReading Osu File from PATH:%s\n", PATH);

    if (!debug)freopen("NUL", "w", stdout);

    FILE* fp = NULL;
    fp = fopen(PATH, "r");

    char buffer[max_length_long];

    // Get Block
    while (fgets(buffer, max_length_long, fp) != NULL)
    {
        printf("BUFFER:%s", buffer);
        if (0 == strcmp("[General]\n", buffer))
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, GENERAL); }
        else if (0 == strcmp("[Editor]\n", buffer))
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, EDITOR); }
        else if (0 == strcmp("[Metadata]\n", buffer))
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, METADATA); }
        else if (0 == strcmp("[Difficulty]\n", buffer))
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, DIFFICULTY); }
        else if (0 == strcmp("[Events]\n", buffer))
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, EVENTS); }
        else if (0 == strcmp("[TimingPoints]\n", buffer))
        {
            osuFilePointer->TimingPointsCnt = 0;
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, TIMINGPOINTS); }
        }
        else if (0 == strcmp("[Colours]\n", buffer))
        {
            osuFilePointer->Colours.ComboCnt = 0;
            osuFilePointer->Colours.SliderBorderFlag = FALSE;
            osuFilePointer->Colours.SliderTrackOverrideFlag = FALSE;
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, COLOURS); }
        }
        else if (0 == strcmp("[HitObjects]\n", buffer))
        {
            osuFilePointer->HitObjectsCnt = 0;
            while (fgets(buffer, max_length_long, fp) && 0 != strcmp(buffer, "\n")) { ProcessLine(osuFilePointer, buffer, HITOBJECTS); }
        }
    }
    fclose(fp);

    if (!debug)freopen("CON", "w", stdout);

    printf("Finished Reading Osu File!\n-------------\n\n");
}