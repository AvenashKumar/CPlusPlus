#pragma once
#include <iostream>
using namespace std;
class sPathHelper
{
public:
	enum eFolder{ LOCAL, LOCALLOW, ROAMING };
public:
	static const string  getAppDataPath(const eFolder p_cFolder = ROAMING);
	static const string getSystemUserName();
	static const string getErrorLogFolderPath();
	static void getParentFolderPath(const string & p_crefstrInputPath, string &p_refstrParentFolderPath);
	static void getParentFolderName(const string & p_crefstrInputPath, string &p_refstrParentFolderName);
	static void getFileName(const string & p_crefstrInputPath, string &p_refstrParentFolderName);
};