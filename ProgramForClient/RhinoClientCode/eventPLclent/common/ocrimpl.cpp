// orcimpl.cpp : 定义 DLL 应用程序的导出函数。
//


#include "ocrimpl.h"

#pragma comment(lib, "libtesseract304.lib")
#pragma comment(lib, "liblept171.lib")


#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
tesseract::TessBaseAPI *g_tessBaseApi = nullptr;
// 这是导出变量的一个示例
int norcimpl = 0;

// 这是导出函数的一个示例。
int fnorcimpl(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 orcimpl.h
Corcimpl::Corcimpl()
{
	return;
}
bool ocrinit(char* _path, char* _name)
{
	if (!(_name))
		return false;
	if (!g_tessBaseApi)
	{
		g_tessBaseApi = new tesseract::TessBaseAPI();
	}

	if (g_tessBaseApi->Init(_path, _name)) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		return false;
	}

	return true;
}
char* ocrText(char* fileName)
{
	if (!g_tessBaseApi)
		return nullptr;

	if (!fileName)
		return nullptr;

	Pix *image = pixRead(fileName);
	if (!image)
		return nullptr;

	char* outText = nullptr;
	g_tessBaseApi->SetImage(image);
	outText = g_tessBaseApi->GetUTF8Text();
	if (outText == nullptr)
		return "";
	else
		return outText;
}

char* ocrText(void* data, int width, int height, int pixSize)
{

	if (!g_tessBaseApi)
		return nullptr;

	if (!data)
		return nullptr;


	char* outText = nullptr;
	g_tessBaseApi->SetImage((const unsigned char*)data, width, height, pixSize, pixSize*width);
	outText = g_tessBaseApi->GetUTF8Text();
	if (outText == nullptr)
		return "";
	else
		return outText;
}