#ifndef ImgMGR_H
#define ImgMGR_H

#define MAX_PATH_LEN 256

extern char res_path[MAX_PATH_LEN];
extern unsigned char* preloaded;

void GetResourceDir();
void CustomLoadResource(unsigned char *file_name);

#endif