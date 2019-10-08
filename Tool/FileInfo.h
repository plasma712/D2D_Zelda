#pragma once
class CFileInfo
{
private:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertRelativePath(const TCHAR* pPath);
	static void ExtractPathInfo(const TCHAR* pPath, list<PATH_INFO*>& rPathInfoLst);
	static int CountImgFiles(const TCHAR* pPath);
};

