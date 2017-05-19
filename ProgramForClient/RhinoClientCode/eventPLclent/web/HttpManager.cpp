
#include "HttpManager.h"
#include "curl/curl.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#pragma comment(lib, "libcurl_imp.lib")

HttpClient::HttpClient(void) :
    m_bDebug(false)
{

}

HttpClient::~HttpClient(void)
{

}

bool HttpClient::PrintCookies(void* curl, std::string& strOut)
{
    std::ostringstream ostr;

    CURLcode res;
    struct curl_slist *cookies;

    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (res != CURLE_OK) {

        ostr << "Curl curl_easy_getinfo failed:" << curl_easy_strerror(res) << std::endl;
        strOut = ostr.str();
        return false;
    }
    const struct curl_slist *nc = cookies;
    int i = 1;
    ostr << "Cookies, curl knows:" << std::endl;
    while (nc) {
        ostr << "[" << i++ << "]: " << nc->data << std::endl;
        nc = nc->next;
    }

    return true;
}

static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
    if (itype == CURLINFO_TEXT)
    {
        //printf("[TEXT]%s\n", pData);
    }
    else if (itype == CURLINFO_HEADER_IN)
    {
        printf("[HEADER_IN]%s\n", pData);
    }
    else if (itype == CURLINFO_HEADER_OUT)
    {
        printf("[HEADER_OUT]%s\n", pData);
    }
    else if (itype == CURLINFO_DATA_IN)
    {
        printf("[DATA_IN]%s\n", pData);
    }
    else if (itype == CURLINFO_DATA_OUT)
    {
        printf("[DATA_OUT]%s\n", pData);
    }
    return 0;
}

size_t OnWriteData_Post(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    std::string* str = reinterpret_cast<std::string*>(lpVoid);
    if (NULL == str || NULL == buffer)
    {
        return -1;
    }

    char* pData = reinterpret_cast<char*>(buffer);
    str->append(pData, size * nmemb);
    return nmemb;
}

size_t OnWriteData_MultipartFormdata( void *inBuffer, size_t size, size_t nmemb, void *outBuffer )
{
    int len = size * nmemb;
    char *temp = new char[len+1];
    memcpy(temp,inBuffer,len);
    temp[len]=0;
    reinterpret_cast<std::string *>(outBuffer)->append(temp);
    delete temp;
    return len;
}

size_t OnWriteData_GetFile(void *inBuffer, int size, int nmemb, std::string &content)
{
    long len = size * nmemb;
    std::string temp((char *)inBuffer, len);
    content += temp;
    return len;
}

std::string HttpClient::getMsgInChinese(int code)
{
    switch(code)
    {
	//case  0:
	//	return  "通讯成功";
	//case  7:
	//	return  "服务器连接失败。";
	//case  28:
	//	return  "连接超时。";
	//case  58:
	//	return  "服务端验证客户端证书失败。";
	//case  60:
	//	return  "客户端验证服务端证书失败。";
	//default:
	//	return  "";
	 
	}
	return "";
}

int HttpClient::Post(const char* pUrl,
                         const char* pPost,
                         std::string & strResponse,
                         bool bPost,
                         const char* pCookie,
                         const char* pCaPath,
                         const char* pClientCalPath,
                         const char* pClientCalPassword)
{
    strResponse = "";


    CURLcode res;
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }
    if (m_bDebug)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
    }
    curl_easy_setopt(curl, CURLOPT_URL, pUrl);
    if(bPost)
    {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pPost);
    }
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData_Post);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    if (pCookie!=0)
    {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, (void *)pCookie);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, (void *)pCookie);
    }

    if (NULL == pCaPath)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        //需要在编译curl的时候，一同编译zlib标志。要不然找不到这个标志.
        //当然前提是你已经编译完成zlib.
        //发出接受gzip压缩内容的请求，如果服务器支持gzip内容，会返回压缩后的数据。
        //如果Http服务器不支持gzip encoding也不影响libcurl正常工作。
        //接受数据的时候，如果返回的是压缩数据，libcurl会自动解压数据。
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
    }
    else
    {
        //缺省情况就是PEM，所以无需设置，另外支持DER
        //curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);

        //服务端需要认证客户端的真实性，即双向认证。
        if(pClientCalPath!=NULL)
        {
            curl_easy_setopt(curl,CURLOPT_SSLCERT, pClientCalPath);
            curl_easy_setopt(curl,CURLOPT_SSLCERTPASSWD, pClientCalPassword);
            curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE, "PEM");
            curl_easy_setopt(curl,CURLOPT_SSLKEY, pClientCalPath);
            curl_easy_setopt(curl,CURLOPT_SSLKEYPASSWD, pClientCalPassword);
            curl_easy_setopt(curl,CURLOPT_SSLKEYTYPE, "PEM");
        }
    }
    //
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

    //Web服务器一般会重定向链接，比如访问http:/xxx/x1.do自动转到http:/xxx/x2.do
    //所以一定要设置CURLOPT_FOLLOWLOCATION为1,否则重定向后的数据不会返回。
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;
}


int HttpClient::Get(const char* pUrl, std::string &strResponse, const char* pCookie,
	const char* pCaPath,
	const char* pClientCalPath,
	const char* pClientCalPassword)
{


	CURLcode res;
	CURL* curl = curl_easy_init();
	if (NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if (m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: Agent-007");
	curl_easy_setopt(curl, CURLOPT_URL, pUrl);
	//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
 
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);

	/* Write callback */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	/* Ioctl function */
	//curl_easy_setopt(curl, CURLOPT_IOCTLFUNCTION, ioctl_callback);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
 
	res = curl_easy_perform(curl);   // 执行
	if (res != 0) {

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	} 
	return res;
}


int HttpClient::MultipartFormdata(const char *pUrl,
                                      const std::map<std::string,std::string> & mapFields,
                                      const std::map<std::string,std::vector<std::string>> & mapFiles,
                                      std::string & strResponse,
                                      const char *pCookie,const char * pCaPath,
                                      const char * pClientCalPath,const char * pClientCalPassword)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    strResponse ="";

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData_MultipartFormdata);//write_data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponse);

    struct curl_httppost *formpost = 0;
    struct curl_httppost *lastptr  = 0;

    //
    std::map<std::string,std::string>::const_iterator iterFields = mapFields.begin();
    while(iterFields!=mapFields.end())
    {
        //curl_formadd(&formpost, &lastptr,CURLFORM_COPYNAME,"data",CURLFORM_COPYCONTENTS, pData,CURLFORM_END);
        curl_formadd(&formpost, &lastptr,CURLFORM_COPYNAME,iterFields->first.c_str(),
                     CURLFORM_COPYCONTENTS, iterFields->second.c_str(),CURLFORM_END);
        iterFields++;
    }

    std::map<std::string,std::vector<std::string>>::const_iterator iterFiles = mapFiles.begin();
    while(iterFiles!=mapFiles.end())
    {
        //"image/jpeg","image/png"
        //curl_formadd(&formpost, &lastptr,CURLFORM_PTRNAME, "portraitFile", CURLFORM_FILE, pImageFileName,CURLFORM_CONTENTTYPE, "image/png", CURLFORM_END);
        curl_formadd(&formpost, &lastptr,CURLFORM_PTRNAME,
                     iterFiles->first.c_str(), CURLFORM_FILE,
                     iterFiles->second[0].c_str(),CURLFORM_CONTENTTYPE,
                iterFiles->second[1].c_str(), CURLFORM_END);
        iterFiles++;
    }


    //
    curl_easy_setopt(curl, CURLOPT_URL, pUrl);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    if (pCookie!=0)
    {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, (void *)pCookie);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, (void *)pCookie);
    }

    //单向认证用
    if(pCaPath!=0)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    }

    //服务端需要认证客户端的真实性，即双向认证。
    if(pClientCalPath!=0 && pClientCalPassword!=0)
    {
        curl_easy_setopt(curl,CURLOPT_SSLCERT, pClientCalPath);
        curl_easy_setopt(curl,CURLOPT_SSLCERTPASSWD, pClientCalPassword);
        curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(curl,CURLOPT_SSLKEY, pClientCalPath);
        curl_easy_setopt(curl,CURLOPT_SSLKEYPASSWD, pClientCalPassword);
        curl_easy_setopt(curl,CURLOPT_SSLKEYTYPE, "PEM");
    }

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    curl_formfree(formpost);
    return res;
}

int HttpClient::GetFile(const char* pUrl,
                            const char* pLocalFullPath,
                            const char* pCookie,
                            const char* pCaPath,
                            const char* pClientCalPath,
                            const char* pClientCalPassword)
{
    CURL *curl = NULL;
    CURLcode code;
    char bufError[CURL_ERROR_SIZE];
    std::string content;
    long retcode = 0;

    code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (code != CURLE_OK)
    {
        //printf("Failed to global init default [%d]\n", code);
        return -100;
    }

    curl = curl_easy_init();
    if (curl == NULL)
    {
        //printf("Failed to create CURL connection\n");
        return -200;
    }
    code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, bufError);
    if (code != CURLE_OK)
    {
        //printf("Failed to set error buffer [%d]\n", code);
        return code;
    }
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    code = curl_easy_setopt(curl, CURLOPT_URL, pUrl);
    if (code != CURLE_OK)
    {
        //printf("Failed to set URL [%s]\n", error);
        goto _END;
    }
    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    if (code != CURLE_OK)
    {
        //printf("Failed to set redirect option [%s]\n", error);
        goto _END;
    }
    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData_GetFile);
    if (code != CURLE_OK)
    {
        //printf("Failed to set writer [%s]\n", error);
        goto _END;
    }
    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    if (code != CURLE_OK)
    {
        //printf("Failed to set write data [%s]\n", error);
        goto _END;
    }

    if (pCookie!=0)
    {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, (void *)pCookie);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, (void *)pCookie);
    }

    //单向认证用
    if(pCaPath!=0)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    }

    //服务端需要认证客户端的真实性，即双向认证。
    if(pClientCalPath!=0 && pClientCalPassword!=0)
    {
        curl_easy_setopt(curl,CURLOPT_SSLCERT, pClientCalPath);
        curl_easy_setopt(curl,CURLOPT_SSLCERTPASSWD, pClientCalPassword);
        curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(curl,CURLOPT_SSLKEY, pClientCalPath);
        curl_easy_setopt(curl,CURLOPT_SSLKEYPASSWD, pClientCalPassword);
        curl_easy_setopt(curl,CURLOPT_SSLKEYTYPE, "PEM");
    }

    code = curl_easy_perform(curl);
    if (code != CURLE_OK)
    {
        //printf("Failed to get '%s' [%s]\n", URL, error);
        goto _END;
    }

    code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);
    if ((code == CURLE_OK) && retcode == 200)
    {
        double length = 0;
        code = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);
        //printf("%d", retcode);
        FILE * file = fopen(pLocalFullPath, "wb");
        fseek(file, 0, SEEK_SET);
        fwrite(content.c_str(), 1, (size_t)length, file);
        fclose(file);

        code =  CURLE_OK;
        goto _END;
    }
_END:
    curl_easy_cleanup(curl);

    return code;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void HttpClient::SetDebug(bool bDebug)
{
    m_bDebug = bDebug;
}

void HttpClient::Init()
{
    //the function is not thread safe.
    curl_global_init(CURL_GLOBAL_ALL);
}

void HttpClient::Cleanup()
{
    curl_global_cleanup();
}


//这个函数是为了符合CURLOPT_READFUNCTION而构造的
//数据上传时使用 
//---------------------
size_t HttpClient::read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	//size_t  amount = nmemb * size; /* Total bytes curl wants */
	//size_t  available = sizeof(databuf)-current_offset; /* What we have to
	//													give */
	//size_t  given = amount < available ? amount : available; /* What is given */
	//(void)stream;
	//memcpy(ptr, databuf + current_offset, given);
	//current_offset += given;
	

	return 0;
}

////这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
//完成数据保存功能

//---------------------
size_t HttpClient::write_callback(void *ptr, size_t size, size_t nmemb,
	void *stream)
{
	int amount = size * nmemb;
	printf("%.*s", amount, (char *)ptr);
	(void)stream;
	return size * nmemb;
}

//
//curlioerr CURLHttpClient::ioctl_callback(CURL * handle, int cmd, void *clientp)
//{
//	(void)clientp;
//	if (cmd == CURLIOCMD_RESTARTREAD) {
//		printf("APPLICATION: recieved a CURLIOCMD_RESTARTREAD request\n");
//		printf("APPLICATION: ** REWINDING! **\n");
//		current_offset = 0;
//		return CURLIOE_OK;
//	}
//	(void)handle;
//	return CURLIOE_UNKNOWNCMD;
//}
//

