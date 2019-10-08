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
	// From���� To�� �������� ����θ� ���� out�� �����ϴ� �Լ�.
	// �� �� From�� To�� ������ ����̺꿡 �����ؾ��Ѵ�.
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
// D:\\������\\99C\\4������\\Framework99C\\Texture\\Stage
void CFileInfo::ExtractPathInfo(const TCHAR* pPath, list<PATH_INFO*>& rPathInfoLst)
{
	wstring wstrFindPath = wstring(pPath) + L"\\*.*";

	// ��� �� ���� Ž���� ���õ� MFC Ŭ����.
	CFileFind find;

	// �ش� ��γ� ������ �����ϴ��� �����ϴ� �Լ�. �����ϸ� TRUE �ƴϸ� FALSE ��ȯ.
	BOOL bIsFind = find.FindFile(wstrFindPath.c_str());

	// FindFile�Լ��� ȣ���ؼ� ���� ������ �˻��ߴٸ� 
	// FindNextFile�Լ��� ȣ���Ͽ� �˻��� ��� ������ �� �ִ�.
	// ����, FindFile�Լ��� ������ �˻� ����� *.*�� ��
	// FindNextFile�Լ��� ȣ���ϸ� �˻� ��� �� ���� ù��° �༮�� �����Ѵ�.		

	while (bIsFind)
	{
		// �� �̻� ã�� ���� ���ų� ������ ����� ã���� �� FALSE�� ��ȯ.
		bIsFind = find.FindNextFile();

		if (find.IsDots()) // ã�� ���� .�̳� ..���� ��Ŀ��� �ǳʶپ��
			continue;
		else if (find.IsDirectory()) // ã���� ������� ���.
			ExtractPathInfo(find.GetFilePath(), rPathInfoLst);
		else
		{
			// ������ ã�Ҵٸ�..
			if (find.IsSystem()) // �����ִ� �ý��� �����̶�� �ǳʶپ��.
				continue;

			////////////////////////////////////////////////////////////////

			PATH_INFO* pPathInfo = new PATH_INFO;

			// ����� ����
			CString strFilePath = find.GetFilePath();

			// CString::Replace(old, new) old -> new ��ü.
			strFilePath.Replace(L"0.", L"%d.");
			pPathInfo->wstrImgPath = ConvertRelativePath(strFilePath);

			/////////////////////////////////////////////////////////////////

			// �̹��� ���� ����

			// strFilePath
			// D:\������\99C\4������\Framework99C\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack%d.png

			TCHAR szFullPath[MAX_STR] = L"";
			lstrcpy(szFullPath, strFilePath);
			::PathRemoveFileSpec(szFullPath);

			// szFullPath
			// D:\������\99C\4������\Framework99C\Texture\Stage\Effect\BossMultiAttack
			pPathInfo->iImgCount = CountImgFiles(szFullPath);

			/////////////////////////////////////////////////////////////////

			// StateKey ����
			// PathFindFileName: ���ڿ����� ���ϸ� ã�� �Լ�. ������ ���� �������� ����.
			pPathInfo->wstrStateKey = ::PathFindFileName(szFullPath);

			/////////////////////////////////////////////////////////////////

			// ObjectKey ����
			::PathRemoveFileSpec(szFullPath);
			pPathInfo->wstrObjectKey = ::PathFindFileName(szFullPath);

			rPathInfoLst.push_back(pPathInfo);
			break;
		}		
	}
}

int CFileInfo::CountImgFiles(const TCHAR* pPath)
{
	// D:\������\99C\4������\Framework99C\Texture\Stage\Effect\BossMultiAttack\*.*
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
