#pragma once
#include <string>
using namespace std;
class sPathUtils
{
public:
	static void getFileName(const string & p_crefstrInputPath, string &p_refstrFileName);
	static void getParentFolderPath(const string & p_crefstrInputPath, string &p_refstrParentFolderPath);
	static void getApplicationName(string &p_refstrApplicationName);
	static void getCurrExecutablePath(string &p_refstrExecutablePath);
	static void getCurrExecutableDirPath(string &p_refstrExecutableDirPath);
};

