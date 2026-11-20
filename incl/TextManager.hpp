#ifndef TxtMGR_H
#define TxtMGR_H

void JsonScriptParser(unsigned char* jsonFilePath);
HRESULT InitWriteFactoryFormat();
HRESULT SetTextFormat(IDWriteTextFormat** textFormat, wchar_t* font_name, float font_size);

#endif