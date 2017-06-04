#include "sPathUtils.h"
#include <Windows.h>

void sPathUtils::getCurrExecutablePath(string &p_refstrExecutablePath)
{
	HMODULE hModule = GetModuleHandleW(NULL);
	CHAR path[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);
	p_refstrExecutablePath = string(path);
}
void sPathUtils::getCurrExecutableDirPath(string &p_refstrExecutableDirPath)
{
	string strExePath = "";
	getCurrExecutablePath(strExePath);
	getParentFolderPath(strExePath, p_refstrExecutableDirPath);
}
void sPathUtils::getParentFolderPath(const string & p_crefstrInputPath, string &p_refstrParentFolderPath)
{
	size_t uiSlashPos = p_crefstrInputPath.rfind(R"(\)");
	if (uiSlashPos == string::npos)
		p_refstrParentFolderPath = p_crefstrInputPath;

	p_refstrParentFolderPath = p_crefstrInputPath.substr(0, uiSlashPos);
}

void sPathUtils::getFileName(const string & p_crefstrInputPath, string &p_refstrFileName)
{
	//Get parent folder path
	string strParentFolderPath = "";
	getParentFolderPath(p_crefstrInputPath, strParentFolderPath);
	size_t uiSizeParentFolderPathSize = strParentFolderPath.size() + 1;

	//Get Parent Folder Name
	p_refstrFileName = p_crefstrInputPath.substr(uiSizeParentFolderPathSize, p_crefstrInputPath.size() - uiSizeParentFolderPathSize);
}

void sPathUtils::getApplicationName(string &p_refstrApplicationName)
{
	string strExeCurrPath = "";
	getCurrExecutablePath(strExeCurrPath);

	getFileName(strExeCurrPath, p_refstrApplicationName);

}