#include <string.h>

#include <dwrite.h>

#include "structs.h"
#include "TextManager.hpp"

// TODO::Release 없음 > 누수 관리 필요
IDWriteFactory* writeFactory = nullptr;

char imgs[1024];
char sounds[1024];
char scripts[4096];

/**
 * @brief Json 파일 안에서 큰 따옴표 ("") 속 내용물을 용도에 맞게 버퍼에 세팅.
 */
void GetQuotedContents(char* src) {
    int identifierStart = 0;
    int identifierEnd = 0;
    int maxLen = strlen(src);
    int bigTag = 0; // 0: none | 1: resources | 2: scripts
    int smallTag = 0; // 
    char identifier[32];
    int offset = 0;

    memset(imgs, '\0', sizeof(imgs));
    memset(sounds, '\0', sizeof(sounds));
    memset(scripts, '\0', sizeof(scripts));

    for (;;) {
        for (int j = 0; (src[offset + j] != '\"'); j++) {
            identifierStart = offset + j + 2;

            if (src[offset + j] == '\0') {
                offset = offset + j;
                break;
            }
        }
        
        for (int j = 0; src[identifierStart + j] != '\"'; j++) {
            identifierEnd = identifierStart + j;

            if (src[offset + j] == '\0') {
                offset = offset + j;
                break;
            }
        }

        if (offset >= maxLen) break;
        offset = identifierEnd + 2;
        
        // 대분류 설정
        if (src[offset+2] == '[') {
            int identifierLen = identifierEnd - identifierStart + 1;
            memcpy(identifier, src + identifierStart, identifierLen);
            identifier[identifierLen] = '\0';
            if (!strcmp(identifier, "resources")) bigTag = 1;
            if (!strcmp(identifier, "scripts")) bigTag = 2;
            continue;
        }

        // 소분류 설정
        if (bigTag == 1 && src[offset+2] == '{') {
            int identifierLen = identifierEnd - identifierStart + 1;
            memcpy(identifier, src + identifierStart, identifierLen);
            identifier[identifierLen] = '\0';
            if (!strcmp(identifier, "imgs")) smallTag = 1;
            if (!strcmp(identifier, "sounds")) smallTag = 2;
            continue;
        }

        if (bigTag == 1 && smallTag == 1) {
            int dataLen = identifierEnd - identifierStart + 1;
            int writeOffset = strlen(imgs);
            memcpy(imgs + writeOffset, src + identifierStart, dataLen);
            imgs[writeOffset+dataLen] = '|';
            continue;
        }

        if (bigTag == 1 && smallTag == 2) {
            int dataLen = identifierEnd - identifierStart + 1;
            int writeOffset = strlen(sounds);
            memcpy(sounds + writeOffset, src + identifierStart, dataLen);
            sounds[writeOffset+dataLen] = '|';
            continue;
        }

        if (bigTag == 2) {
            int dataLen = identifierEnd - identifierStart + 1;
            int writeOffset = strlen(scripts);
            memcpy(scripts + writeOffset, src + identifierStart, dataLen);
            scripts[writeOffset+dataLen] = '|';
            continue;
        }
    }
}

void JsonScriptParser(unsigned char* jsonFilePath) {
    FILE *fp = fopen((const char*)jsonFilePath, "r");

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);

    size_t readSize = fread(buffer, sizeof(char), fileSize, fp);
    buffer[readSize] = '\0';

    int offset = 0;

    GetQuotedContents(buffer);

    fclose(fp);
    free(buffer);
}

int GetImgPathFromBuffer(unsigned char* imgPath, int start_offset) {
    int offset = 0;

    if (imgs[start_offset] == '\0') return -1;

    for (int i = 0; ;i++) {
        if (imgs[start_offset + i] == '|') {
            offset = start_offset + i + 1;
            break;
        }
    }

    int i = 0;
    while(imgs[offset] != '|') {
        imgPath[i++] = imgs[offset++];
    }
    imgPath[i] = '\0';
    
    return offset += 2;
}

void GetSoundPathFromBuffer() {
    
}

void GetScriptPathFromBuffer() {
    
}

HRESULT InitWriteFactoryFormat() {

    writeFactory = nullptr;
    
    HRESULT hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&writeFactory)
    );
    return hr;
}

HRESULT SetTextFormat(IDWriteTextFormat** textFormat, wchar_t* font_name, float font_size) {

    *textFormat = nullptr;

    HRESULT hr = writeFactory->CreateTextFormat(
            font_name,
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            font_size,
            L"ko-kr",
            textFormat
        );

    return hr;
}