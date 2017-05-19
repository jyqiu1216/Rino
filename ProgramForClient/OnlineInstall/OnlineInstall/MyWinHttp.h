
#ifndef WJ_HttpClient_h
#define WJ_HttpClient_h

#define HTTP_WEB_ADDRESS_SERVER	L"118.178.25.88"
#define HTTP_WEB_ADDRESS_SERVER2	L"api.xiniugame.com"/*L"118.178.25.88"*/

class CMainWnd;

class CMyWinHttp
{
public:
	CMyWinHttp(void);
	~CMyWinHttp(void);

	//通知服务器下载安装成功
	void tellHttpServerDownFinish(int& iSource);
	//通知服务器下载安装成功_测试
	void tellHttpServerDownFinish_test(char* sHDSerial);
	//获取当前最新版本下载地址
	void GetHttpDownAddress(std::string &str_res);

	void SetpCMainWnd(CMainWnd* pCMainWnd){ m_pCMainWnd = pCMainWnd; }
private:
	void GetHttpData(std::wstring str_web_addr, int iport, std::wstring str_web_content, std::string &str_res);

	CMainWnd* m_pCMainWnd;
};

#endif//WJ_HttpClient_h