#include "stdafx.h"
#include "UnzipFile.h"


CUnzipFile::CUnzipFile()
{

}

CUnzipFile::~CUnzipFile()
{

}

bool CUnzipFile::UnZipFile(CPaintManagerUI* pManager, LPCTSTR lptZip, int inFileSize, LPCTSTR lptDir)
{
	bool bRet = false;

	try {

		if ((NULL == pManager) || (NULL == lptZip) || (inFileSize <= 0) || (NULL == lptDir)) {

			return bRet;
		}

		wchar_t wchTemp[256] = { 0 };
		::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("正在解压安装包，请稍候..."));

		static CTextUI* pText = NULL;

		if (NULL == pText) {

			pText = static_cast<CTextUI*>(pManager->FindControl(TEXT("Text.Download")));
		}

		if (pText) {

			pManager->SendNotify(pText, TEXT("Msg.DwonLoad.Speed"), (WPARAM)pText, (LPARAM)wchTemp);
		}

		namespace fs = boost::filesystem;
		fs::path path(lptDir);
		wstring wstrParentPath = TEXT("");

		if (::wcsstr(lptDir, TEXT("\\RhinoClient"))) {

			wstrParentPath = path.parent_path().c_str();
		}
		else {

			wstrParentPath = lptDir;
		}

		wchar_t wchCurDirectory[1024] = { 0 };
		if (::GetCurrentDirectory(ARRAYSIZE(wchCurDirectory), wchCurDirectory) <= 0) {

			return bRet;
		}

		::SetCurrentDirectory(wstrParentPath.c_str());
		HZIP hZip = OpenZip((void*)lptZip, 0, ZIP_FILENAME);

		if (NULL != hZip) {

			wchar_t wchFilePath[1024] = { 0 };
			ZIPENTRY ZipEntry = { 0 };
			GetZipItem(hZip, -1, &ZipEntry);

			for (int i = 0; i < ZipEntry.index; i++) {

				ZIPENTRY tZipEntry = { 0 };

				if (ZIP_OK == GetZipItem(hZip, i, &tZipEntry)) {

					::ZeroMemory(wchFilePath, ARRAYSIZE(wchFilePath));
					::swprintf_s(wchFilePath, ARRAYSIZE(wchFilePath), TEXT("%ws\\%ws"), wstrParentPath.c_str(), tZipEntry.name);

					if (FILE_ATTRIBUTE_DIRECTORY & tZipEntry.attr) {

						if (!fs::is_directory(wchFilePath)) {

							fs::create_directories(wchFilePath);
						}
					}

					UnzipItem(hZip, i, wchFilePath, 0, ZIP_FILENAME);
				}
			}
		}

		CloseZip(hZip);
		::SetCurrentDirectory(wchCurDirectory);

		::DeleteFile(lptZip);
		bRet = true;
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}
