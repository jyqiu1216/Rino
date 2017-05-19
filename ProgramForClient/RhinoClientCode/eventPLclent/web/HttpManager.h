#ifndef _HTTPMANAGER_H_
#define _HTTPMANAGER_H_


//HttpClient.h源代码清单

#include <string>
#include <map>
#include <vector>

/*
Title: Get Response from Web Server by HTTP/HTTPS method.
Environment:
Windows 7SP1, Windows 8.1, Windows 10
QT Creator 3.5.1, Visual Studio 2013 Update1, Visual Studio 2013 Update5
libcurl 7.36.0, libcurl 7.46.0, Qt 5.6, MSYS2 64bits gcc 5.3.0
Last Update: 2016-05-27
Remark:
[1]如果要在多线程方式下同时调用多个CURLHttpClient实例，
需要在App初始化的时候调用kagula::network::Init();
在App结束的时候调用kagula::network::Cleanup();
[2]编译libcurl必须打开zlib标志，并且把OpenSSL也链进去。

Reference:
curl_eay_setopt manual
http://www.helplib.net/s/linux.die/65_2740/man-3-curl-easy-setopt.shtml
C++ cout format
http://www.cnblogs.com/devymex/archive/2010/09/06/1818754.html
*/



class HttpClient
{
public:
	HttpClient(void);
	~HttpClient(void);

public:
	/**
	* @brief HTTP/HTTPS POST/GET请求
	* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
	* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
	* @param strCookie 输入参数,Cookie文件名，例如 d:\temp\cookie.txt
	*                          如果为空，不启用Cookie.
	* @param strResponse 输出参数,返回的内容
	* @param bPost 是否Post方式发送请求，默认Post方式发送请求。
	* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	* @param pClientCalPath 输入参数，为客户端证书的路径.如果输入为NULL,则不验证客户端证书的有效性.
	* @param pClientCalPassword 输入参数，为客户端证书的存取密码.
	* @return 返回是否Post成功
	* 0  成功
	* 7  无法连接
	* 28 超时
	* 58 服务端验证客户端证书失败。
	* 60 客户端验证服务端证书失败。
	*/
	int Post(const char* pUrl,
		const char* pPost,
		std::string &strResponse,
		bool bPost,
		const char* pCookie,
		const char* pCaPath = NULL,
		const char* pClientCalPath = NULL,
		const char* pClientCalPassword = NULL);

	int Get(const char* pUrl, std::string &strResponse, const char* pCookie = nullptr,
		const char* pCaPath = NULL,
		const char* pClientCalPath = NULL,
		const char* pClientCalPassword = NULL);


	int MultipartFormdata(const char *pUrl,
		const std::map<std::string, std::string> & mapFields,
		const std::map<std::string, std::vector<std::string>> & mapFiles,
		std::string & strResponse,
		const char *pCookie,
		const char * pCaPath = NULL,
		const char * pClientCalPath = NULL,
		const char * pClientCalPassword = NULL);

	int GetFile(const char* pUrl,
		const char* pLocalFullPath,
		const char* pCookie,
		const char* pCaPath = NULL,
		const char* pClientCalPath = NULL,
		const char* pClientCalPassword = NULL);
public:
	void SetDebug(bool bDebug);
	std::string getMsgInChinese(int code);

	static   void Init();
	static   void Cleanup();

	static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream);
//	static curlioerr ioctl_callback(CURL * handle, int cmd, void *clientp);
private:
	bool m_bDebug;
	std::string m_strResponse;
	size_t current_offset;
	bool PrintCookies(void* curl, std::string& strOut);
};

#endif // _HTTPMANAGER_H_
