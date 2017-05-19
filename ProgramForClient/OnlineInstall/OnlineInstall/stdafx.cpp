// stdafx.cpp : 只包括标准包含文件的源文件
// OnlineInstall.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用


char char_info[] = {"\
那一瞬间即永恒\n\
\n\
一瞬间的那种感觉是最真的感觉。那一瞬间涌现的是人世间千万种的情感。那一瞬间是精彩的，是美好的，也是永恒的。\n\
\n\
——题记\n\
\n\
汨罗江畔，乘一叶扁舟，无意识的停留，听闻屈大夫深情的吟唱着“举世皆浊我独清，众人皆醉我独醒”的歌谣。苍老的江风拂过他泛白的两鬓，步行于江边，看苍茫间那一丝短暂的光明。撑着《离骚》歌韵的船只，摇动《九歌》和《九章》的双桨，升起《天问》不满的征帆，汨罗江水边，又有谁明白在他心中交织着生与义的苦痛呢？他，驻立在江边，风轻轻拂过，在生与义之间，做出了单项选择。在激荡的风浪中，在扁舟远逝于江边的那一瞬间，已化作永恒。\n\
\n\
“路漫漫其修远兮，吾将上下而求索。”这是屈原对自己理想的最好诠释。作为一个楚国之人，他将自己的热忱献给了自己热爱的祖国；作为一个楚国之臣，作文网在他多次被贬之后把自己的生命献给了人民；作为一个文人，他把“虽九死其犹未悔”作为自己的准则。“举贤而援能兮，循绳墨而不顾”是他的追求。“忍而不能舍”是他的态度。“既莫足与为美政兮。吾将从彭咸之所居”是他最后的吟唱。他用自己的生命诠释了自己永恒的一瞬间。\n\
\n\
那一瞬间即永恒。\n\
\n\
	听，破旧的居室中传来一阵美妙的旋律。这是一曲以坚定为曲，以毅力为谱的生命之歌。贝多芬，上帝为他关上了窗，也为他关上了门，而他却用自己的毅力凿出了一个洞。在钢琴边苦练，紧扼住命运的咽喉，一个人承担着无尽的打击与压力。在无声的世界中牵手音乐，在五线谱上舞动出生命的价值，在钢琴键上创造出人性的高洁。他的每一首曲子都是生活的强者对命运发出的宣战书，在这样的音乐巨人面前世人皆顶礼膜拜。在那一曲《第九交响曲》传遍世界的那一瞬间，已化作永恒。\n\
\n\
	“我要扼住命运的咽喉，它妄想使我屈服，这绝对办不到。”这是贝多芬一生的座右铭，亦是他一生最真实的写照。“音乐给人快乐，热爱音乐的人一定热爱生活”他是一个热爱音乐的人，更是一个热爱生活的人。\n\
\n\
	那一瞬间即永恒。\n\
\n\
	时钟可以倒转，但时间可以么？花儿可以重开，但人还有轮回么？永恒的东西，仅管时间短暂，但从它闪光的那一刹那起，它并注定那么美丽，那么不平凡。而这“一瞬间”或许会在不经意间烙在了历史卷轴的末端，而历史的星空或许也会因为这许许多多一瞬间的永恒而美丽和厚重！\n\
\n\
	其实，那一瞬间即永恒。\
"};

int IsTextUTF8(const char* str, ULONGLONG length)
{
	int i;
	DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UCHAR chr;
	BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8
	for (i = 0; i < length; i++)
	{
		chr = *(str + i);
		if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii = FALSE;
		if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if ((chr & 0xC0) != 0x80)
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	if (nBytes > 0) //违返规则
	{
		return FALSE;
	}
	if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
	{
		return FALSE;
	}
	return TRUE;
}

//Utf8ToGb2312
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//std::string 转 UTF - 8：
char *  string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	return pBuf;
}
//std::wstring 转 UTF - 8：
char * wstring_To_UTF8(const std::wstring & str)
{
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), nLen, pBuf, nLen, NULL, NULL);
	return pBuf;
}

//UTF - 8 转 std::string：
char* UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	return pBuf;

}

 std::string GetAppPath()
{
	//char lpBuffer[MAX_PATH];
	//ZeroMemory(lpBuffer, sizeof(lpBuffer));
	//GetCurrentDirectoryA(MAX_PATH, lpBuffer);

	char szFilePath[MAX_PATH + 1];
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
	std::string ss = szFilePath;

	//std::string ss = lpBuffer;
	//ss += "\\";
	return ss;
}