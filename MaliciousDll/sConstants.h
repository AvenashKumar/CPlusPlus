#pragma once
#include <string>
#include "sPathHelper.h"
using namespace std;
class sConstants
{
public:
	static string ROOT_PATH_COOKIES_CHROME;
	static string ROOT_PATH_COOKIES_FIREFOX;
	static string PATH_DEST;
	static string getZipCookiesPath();
	static string STR_ERROR;
	static string STR_WARNING;
};