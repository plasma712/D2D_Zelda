#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const TCHAR* pPath)
{
	// From에서 To로 가기위한 상대경로를 만들어서 out에 저장하는 함수.
	// 이 때 From과 To는 동일한 드라이브에 존재해야한다.
	/*::PathRelativePathTo(out, From, FILE_ATTRIBUTE_DIRECTORY,
		To, FILE_ATTRIBUTE_DIRECTORY);*/

	TCHAR szCurrentPath[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	TCHAR szRelativePath[MAX_STR] = L"";
	::PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		pPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

// pPath
// D:\\김태윤\\99C\\4개월차\\Framework99C\\Texture\\Stage
void CFileInfo::ExtractPathInfo(const TCHAR* pPath, list<PATH_INFO*>& rPathInfoLst)
{
	wstring wstrFindPath = wstring(pPath) + L"\\*.*";

	// 경로 및 파일 탐색에 관련된 MFC 클래스.
	CFileFind find;

	// 해당 경로나 파일이 존재하는지 조사하는 함수. 존재하면 TRUE 아니면 FALSE 반환.
	BOOL bIsFind = find.FindFile(wstrFindPath.c_str());

	// FindFile함수를 호출해서 존재 유무를 검사했다면 
	// FindNextFile함수를 호출하여 검색된 대상에 접근할 수 있다.
	// 만약, FindFile함수에 전달한 검색 대상이 *.*일 때
	// FindNextFile함수를 호출하면 검색 대상 중 제일 첫번째 녀석에 접근한다.		

	while (bIsFind)
	{
		// 더 이상 찾을 것이 없거나 마지막 대상을 찾았을 때 FALSE를 반환.
		bIsFind = find.FindNextFile();

		if (find.IsDots()) // 찾은 것이 .이나 ..같은 마커라면 건너뛰어라
			continue;
		else if (find.IsDirectory()) // 찾은게 폴더라면 재귀.
			ExtractPathInfo(find.GetFilePath(), rPathInfoLst);
		else
		{
			// 파일을 찾았다면..
			if (find.IsSystem()) // 숨어있는 시스템 파일이라면 건너뛰어라.
				continue;

			////////////////////////////////////////////////////////////////

			PATH_INFO* pPathInfo = new PATH_INFO;

			// 상대경로 추출
			CString strFilePath = find.GetFilePath();

			// CString::Replace(old, new) old -> new 교체.
			strFilePath.Replace(L"0.", L"%d.");
			pPathInfo->wstrImgPath = ConvertRelativePath(strFilePath);

			/////////////////////////////////////////////////////////////////

			// 이미지 개수 추출

			// strFilePath
			// D:\김태윤\99C\4개월차\Framework99C\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack%d.png

			TCHAR szFullPath[MAX_STR] = L"";
			lstrcpy(szFullPath, strFilePath);
			::PathRemoveFileSpec(szFullPath);

			// szFullPath
			// D:\김태윤\99C\4개월차\Framework99C\Texture\Stage\Effect\BossMultiAttack
			pPathInfo->iImgCount = CountImgFiles(szFullPath);

			/////////////////////////////////////////////////////////////////

			// StateKey 추출
			// PathFindFileName: 문자열에서 파일명 찾는 함수. 없으면 말단 폴더명을 얻어옴.
			pPathInfo->wstrStateKey = ::PathFindFileName(szFullPath);

			/////////////////////////////////////////////////////////////////

			// ObjectKey 추출
			::PathRemoveFileSpec(szFullPath);
			pPathInfo->wstrObjectKey = ::PathFindFileName(szFullPath);

			rPathInfoLst.push_back(pPathInfo);
			break;
		}		
	}
}

int CFileInfo::CountImgFiles(const TCHAR* pPath)
{
	// D:\김태윤\99C\4개월차\Framework99C\Texture\Stage\Effect\BossMultiAttack\*.*
	wstring wstrFindPath = wstring(pPath) + L"\\*.*";

	CFileFind find;
	BOOL bIsFind = find.FindFile(wstrFindPath.c_str());

	int iCount = 0;

	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if(find.IsDots() || find.IsDirectory() || find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;
}
