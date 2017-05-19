#include "AMTest.h"
#include <QtWidgets/QApplication>



extern "C" { _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
extern "C" { __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; }

#include <Windows.h>
#include <iostream>

using namespace std;
std::string Utf8ToStr(const std::string& _utf8)
{
	static char sBuffer[1024];
	static wchar_t sBufferTemp[1024];

	ZeroMemory(sBufferTemp, 1024);
	ZeroMemory(sBuffer, 1024);

	int len = MultiByteToWideChar(CP_UTF8, 0, _utf8.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, _utf8.c_str(), _utf8.length(), sBufferTemp, len);

	int length = WideCharToMultiByte(CP_ACP, 0, sBufferTemp, -1, NULL, NULL, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, sBufferTemp, len, sBuffer, length, NULL, NULL);

	return std::string(sBuffer);
}

#include "ocrimpl.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AMTest w;
	w.move(20, 200);
	w.show();
	return a.exec();
}
