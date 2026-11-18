#include <dwrite.h>

#include "structs.h"
#include "TextManager.hpp"

char description[256];
char resources[1024];
char scripts[4096];

/**
 * @brief Json 파일 안에서 큰 따옴표 ("") 속 내용물을 가져온다.
 * @note count 만큼 파싱해 오고, 마지막 큰 따옴표 다음 offset을 반환.
 * @param count 파싱해 올 대상 수 (목표)
 */
int GetQuotedContents(char* src, char* dst, int count) {
    int identifierStart = 0;
    int identifierEnd = 0;
    int contentCount = 0;
    int maxLen = strlen(src);
    int writeOffset = 0;

    for (int offset = 0; contentCount < count && offset <= maxLen; ) {
        for (int j = 0; (src[offset + j] != '\"'); j++) {
            identifierStart = offset + j + 2;
            if (src[offset + j] == '\0') offset = j;
        }
        
        for (int j = 0; src[identifierStart + j] != '\"'; j++) {
            identifierEnd = identifierStart + j;
            if (src[offset + j] == '\0') offset = j;
        }

        int identifierLen = identifierEnd - identifierStart + 1;
        memcpy(dst + writeOffset, src + identifierStart, identifierLen);

        contentCount++;
        writeOffset += identifierLen;
        offset = identifierEnd + 2;
    }
    return identifierEnd + 2;
}

int GetContentsNum(char* src) {

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

    offset += GetQuotedContents(buffer + offset, description, 4); // Json 스크립트 요약 부분 파싱
    offset += GetQuotedContents(buffer + offset, resources, 7); // Json 스크립트 리소스 부분 파싱
    offset += GetQuotedContents(buffer + offset, scripts, 25); // Json 스크립트 대사 스크립트 부분 파싱


    fclose(fp);
    free(buffer);
}