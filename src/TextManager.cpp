#include <dwrite.h>

#include "structs.h"
#include "TextManager.hpp"

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
            continue;
        }

        if (bigTag == 1 && smallTag == 2) {
            int dataLen = identifierEnd - identifierStart + 1;
            int writeOffset = strlen(sounds);
            memcpy(sounds + writeOffset, src + identifierStart, dataLen);
            continue;
        }

        if (bigTag == 2) {
            int dataLen = identifierEnd - identifierStart + 1;
            int writeOffset = strlen(scripts);
            memcpy(scripts + writeOffset, src + identifierStart, dataLen);
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