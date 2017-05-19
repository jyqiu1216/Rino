#pragma once

#ifndef UNZIPFILE_H
#define UNZIPFILE_H

class CUnzipFile {

private:
protected:
public:
	CUnzipFile();
	~CUnzipFile();

	//½âÑ¹ÎÄ¼þ
	bool																					UnZipFile(CPaintManagerUI* pManager, LPCTSTR lptZip, int inFileSize, LPCTSTR lptDir);
};

#endif