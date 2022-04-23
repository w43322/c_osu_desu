#pragma once

#include "global.h"
#include "filelist.h"
#include "zip.h"
#include "dirent.h"

int on_extract_entry(const char* filename, void* arg);
void GetOszFiles(FileList* fl);
void ExtractOszFiles(FileList* fl);
void Extract();