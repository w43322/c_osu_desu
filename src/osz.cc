#include "osz.h"

int on_extract_entry(const char* filename, void* arg)
{
    static int i = 0;
    printf("Extracted: %s (%d th file)\n", filename, ++i);
    return 0;
}

void GetOszFiles(FileList* fl)
{
    DIR* d;
    struct dirent* dir;
    d = opendir(".\\import");

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char* rdot = strrchr(dir->d_name, '.');
            if (rdot && strcmp(rdot, ".osz") == 0)
            {
                FileListPushBack(fl, dir->d_name);
                printf("osu file read:%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
}

void ExtractOszFiles(FileList* fl)
{
    fl = fl->next;
    while (fl)
    {
        char arg1[256] = ".\\import\\";
        strcat(arg1, fl->path);
        char arg2[256] = ".\\beatmapsets\\";
        sscanf(fl->path, "%s", arg2 + 14);
        printf("arg1:%s |||| arg2:%s\n", arg1, arg2);
        zip_extract(arg1, arg2, on_extract_entry, 0);

        fl = fl->next;
    }
}

void Extract()
{
    FileList* head;
    FileListInit(&head);
    GetOszFiles(head);//读取import目录下的osz文件
    ExtractOszFiles(head);//解压缩osz文件
}