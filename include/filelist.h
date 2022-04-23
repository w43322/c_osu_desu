#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct FILELIST
{
    char path[256];
    struct FILELIST *next;
}FileList;

void FileListInit(FileList** fl);
void FileListPushBack(FileList* fl, const char* nextpath);