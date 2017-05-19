

class CMyFileOperation
{
public:
	CMyFileOperation();
	~CMyFileOperation();

	BOOL DeleteFolder(LPCTSTR lpszPath);
	BOOL CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
	BOOL MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
	BOOL ReNameFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
private:

};