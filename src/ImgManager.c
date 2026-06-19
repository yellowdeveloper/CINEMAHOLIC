#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "ImgManager.h"

char* getResourceDir() {
    GetCurrentDirectoryA(255, res_path);
    strcat_s(res_path, sizeof(res_path), "\\res");
}
