#include <Shlobj.h>
#include "sPathHelper.h"
#include <Windows.h>
#include <atlbase.h>
#include <atlconv.h>
#include <string>
#include "sConstants.h"

const string sPathHelper::getAppDataPath(const eFolder p_cFolder /*= ROAMING*/)
{
	LPWSTR lpstrAppDataPath = NULL;

	HRESULT hr;
	switch (p_cFolder)
	{
	case LOCAL:
		hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &lpstrAppDataPath);
		break;
	case LOCALLOW:
		hr = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, NULL, &lpstrAppDataPath);
		break;
	case ROAMING:
		hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &lpstrAppDataPath);
		break;
	default:
		break;
	}

	if (!SUCCEEDED(hr))
	{
		CoTaskMemFree(lpstrAppDataPath);
		throw exception("Fail to reterive app data path");
	}

	string strAppDataPath = CW2A(lpstrAppDataPath);

	CoTaskMemFree(lpstrAppDataPath);

	return strAppDataPath;
}

const string sPathHelper::getSystemUserName()
{
	char chstrUsername[233];
	DWORD dwordLength = 1024;
	if (!GetUserName(chstrUsername, &dwordLength))
	{
		string strError = "Fail to find current user name, Error Code:=" + std::to_string(GetLastError());
		throw exception(strError.c_str());
	}
	string strUsername = string(chstrUsername);

	return strUsername;
}

const string sPathHelper::getErrorLogFolderPath()
{
	return  sConstants::PATH_DEST;
}
void sPathHelper::getParentFolderPath(const string & p_crefstrInputPath, string &p_refstrParentFolderPath)
{
	size_t uiSlashPos = p_crefstrInputPath.rfind(R"(\)");
	if (uiSlashPos == string::npos)
		p_refstrParentFolderPath = p_crefstrInputPath;

	p_refstrParentFolderPath = p_crefstrInputPath.substr(0, uiSlashPos);
}

void sPathHelper::getParentFolderName(const string & p_crefstrInputPath, string &p_refstrParentFolderName)
{
	//Get parent folder path
	string strParentFolderPath = "";
	getParentFolderPath(p_crefstrInputPath, strParentFolderPath);
	size_t uiSizeParentFolderPathSize = strParentFolderPath.size();

	//get grand parent folder path
	getParentFolderPath(strParentFolderPath, p_refstrParentFolderName);
	size_t uiSizeGrandParentFolderPathSize = p_refstrParentFolderName.size() + 1;

	//Get Parent Folder Name
	p_refstrParentFolderName = p_crefstrInputPath.substr(uiSizeGrandParentFolderPathSize, uiSizeParentFolderPathSize - uiSizeGrandParentFolderPathSize);
}

void sPathHelper::getFileName(const string & p_crefstrInputPath, string &p_refstrParentFolderName)
{
	//Get parent folder path
	string strParentFolderPath = "";
	getParentFolderPath(p_crefstrInputPath, strParentFolderPath);
	size_t uiSizeParentFolderPathSize = strParentFolderPath.size() + 1;

	//Get Parent Folder Name
	p_refstrParentFolderName = p_crefstrInputPath.substr(uiSizeParentFolderPathSize, p_crefstrInputPath.size() - uiSizeParentFolderPathSize);
}
