#include <stdio.h>
#include <string.h>

#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "ImgManager.h"

char* GetResourceDir() {
    GetCurrentDirectoryA(255, res_path);
    strcat_s(res_path, sizeof(res_path), "\\res");
}


void CustomLoadResource(unsigned char *file_name) {
    char file_path[256] = {0};
    
    sprintf_s(file_path, 256, "%s\\%s", res_path, file_name);

    int imgWidth, imgHeight, imgChannels;

    unsigned char *data = stbi_load(file_path, &imgWidth, &imgHeight, &imgChannels, 0);

    if (data) {
        printf("Load Success, H:%d, W:%d, C:%d", imgHeight, imgWidth, imgChannels);
    }
    else {
        printf("Load Error");
    }
}
