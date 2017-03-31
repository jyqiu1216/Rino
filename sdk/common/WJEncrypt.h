#ifndef WJ_ENCRYPT
#define WJ_ENCRYPT

#define ENCRYPT_KEY 729520  // 加密密码
#define FILE_NAME_MAX_SIZE 1000  // 文件名长度 

// 变换法则（用于资源加密）已知文件大小
static int genFun(int size, int key, int i)
{
	return size + key + i - size / key;
}

// 变换法则（用于日志加密）不确定文件大小
static int genFunEx(int key)
{
	return key;
}

#endif//WJ_ENCRYPT