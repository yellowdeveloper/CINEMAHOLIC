#ifndef ImgMGR_H
#define ImgMGR_H

extern wchar_t res_path[256];

void GetResourceDir();
IWICBitmapFrameDecode* CustomLoadImage(wchar_t* img_name);

#endif