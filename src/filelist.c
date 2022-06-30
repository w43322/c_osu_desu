#include "filelist.h"

void FileListInit(FileList** fl)
{
    *fl = (FileList*)malloc(sizeof(struct FILELIST));
    strcpy((*fl)->path, "$HEAD$");
    (*fl)->next = NULL;
}

void FileListPushBack(FileList* fl, const char* nextpath)
{
    while (fl->next != NULL)
        fl = fl->next;
    FileList* now = (FileList*)malloc(sizeof(struct FILELIST));
    fl->next = now;
    strcpy(now->path, nextpath);
    now->next = NULL;
}