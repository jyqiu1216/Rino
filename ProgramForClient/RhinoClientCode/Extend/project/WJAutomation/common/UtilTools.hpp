#ifndef _UtilTools_H_
#define _UtilTools_H_

#include <Windows.h>
#include <shellapi.h>
#include <string>
using namespace std;

/**
* \brief： 释放内存，同时设置指针为nullptr
*/
#define DELETE_PTR(p) \
	if(p) { delete p; p=nullptr; }

/**
* \brief： 当 p 为空指针时, 当前操作 break (如 for循环、while循环)
*/
#define BREAK_IF_NULL(p) \
	if(!p) break;

/**
* \brief： 当 p 为空指针时, 当前函数体 return
*/
#define RETURN_IF_NULL(p) \
	if(!p) return;

/**
* \brief： 当 p 为空指针时, 当前函数体 return false
*/
#define RETURN_FALSE_IF_NULL(p) \
	if(!p) return false;


/**
* \brief:
*/
#define RETURN_NULL_IF_NULL(p)	\
	if(!p) return 0;



#define MATH_PI		3.14159265358979323846
#define MATH_PIF	3.141593f


#define WIN32_CLASSNAME	"Win32-Window"

class UtilTools
{
public:
	static HWND createWin32Window(char* _title, int _x, int _y, int _w, int _h, WNDPROC _windowProc)
	{
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(wcex));
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = _windowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandleW(NULL);
		wcex.hIcon = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
		wcex.hCursor = LoadCursor(0, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //CreateSolidBrush(RGB(255, 99, 71));//;
		wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
		wcex.lpszClassName = WIN32_CLASSNAME;
		//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		if (!RegisterClassEx(&wcex))
			return NULL;

		HWND hWnd = CreateWindowEx(WS_EX_LAYERED, WIN32_CLASSNAME, _title, WS_POPUP | WS_VISIBLE,
			_x, _y, _w, _h, NULL, NULL, GetModuleHandleW(NULL), NULL);


		return hWnd;
	}

	/**
	* \brief:	根据范围 [_min, _max] 对变量值进行修正，使其不超过该范围
	* \param (_v):	需修正的变量
	* \param (_min):	最小范围值
	* \param (_max):	最大范围值
	* \return:	void
	*/
	template <class T>
	static void reviseValue(T& _v, T _min, T _max)
	{
		if (_v < _min)
		{
			_v = _min;
		}
		else if (_v > _max)
		{
			_v = _max;
		}
	}

	/**
	* \brief:	根据最小范围对变量值进行修正，使其不小于该范围
	* \param (_v):	需修正的变量
	* \param (_min):	最小范围值
	* \return:	void
	*/
	template <class T>
	static void reviseValueMin(T& _v, T _min)
	{
		if (_v < _min)
			_v = _min;
	}

	/**
	* \brief:	根据最大范围对变量值进行修正，使其不大于该范围
	* \param (_v):	需修正的变量
	* \param (_max):	最大范围值
	* \return:	void
	*/
	template <class T>
	static void reviseValueMax(T& _v, T _max)
	{
		if (_v > _max)
			_v = _max;
	}

	static size_t formatString(std::string& _str, const char *_format, ...)
	{
		static char sBuffer[1000];
		memset(sBuffer, 0x00, sizeof(sBuffer));

		va_list vaList;
		va_start(vaList, _format);

		try
		{
			vsnprintf_s(sBuffer, 1000, _format, vaList);
		}
		catch (...)
		{
			throw 0;
			return 0;
		}

		va_end(vaList);
		_str = sBuffer;

		while (_str.length() % 4 != 0)
		{
			_str += " ";
		}

		return _str.length();
	}

	static std::string strToUtf8(const std::string& _str)
	{
		static char sBuffer[1024];
		static wchar_t sBufferTemp[1024];

		ZeroMemory(sBufferTemp, 1024);
		ZeroMemory(sBuffer, 1024);

		int len = MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _str.c_str(), _str.length(), sBufferTemp, len);

		int length = WideCharToMultiByte(CP_UTF8, 0, sBufferTemp, -1, NULL, NULL, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, sBufferTemp, len, sBuffer, length, NULL, NULL);
		return std::string(sBuffer);
	}

	static std::string utf8ToStr(const std::string& _utf8)
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


	static int strToInt(const char* _str)
	{
		return atoi(_str);
	}

	static char* intToStr(int _val)
	{
		static char buffer[128];
		memset(buffer, '\0', 128);
		_itoa_s(_val, buffer, 10);
		return buffer;
	}


	static void loadWebUrl(const char *_url)
	{
		ShellExecuteA(NULL, "open", _url, NULL, NULL, SW_SHOW);
	}

	static void* loadImage(const char* _fileName, unsigned int& width, unsigned int& height)
	{
		FILE* f = NULL;
		fopen_s(&f, _fileName, "rb");
		if (!f)
			return nullptr;

		fseek(f, 0x0012, SEEK_SET);
		fread(&width, 4, 1, f);		// 读取宽度
		fread(&height, 4, 1, f);	// 读取高度度
		fseek(f, 0, SEEK_END);
		
		unsigned int size = ftell(f) - 54;
		void* data = (unsigned char*)malloc(size);	// 分配保存数据的内存空间
		fseek(f, 54, SEEK_SET);    
		fread(data, size, 1, f);	// 读取数据
		fclose(f);
		return data;
	}
};



#endif // _UtilTools_H_