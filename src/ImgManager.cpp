#include <stdio.h>
#include <string.h>

#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "ImgManager.hpp"

char res_path[MAX_PATH_LEN];
extern unsigned char* preloaded = NULL;

void GetResourceDir() {
    GetCurrentDirectoryA(MAX_PATH_LEN, res_path);
    strcat_s(res_path, MAX_PATH_LEN, "\\res");
}

void CustomLoadResource(unsigned char *file_name) {
    char file_path[MAX_PATH_LEN] = {0};
    
    sprintf_s(file_path, MAX_PATH_LEN, "%s\\%s", res_path, file_name);

    int imgWidth, imgHeight, imgChannels;

    unsigned char *data = stbi_load(file_path, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);

    if (data) {
        printf("Load Success, H:%d, W:%d, C:%d", imgHeight, imgWidth, imgChannels);
        preloaded = data;
    }
    else {
        printf("Load Error");
        preloaded = NULL;
    }
}

// TODO: implement preloaded image que (circular)