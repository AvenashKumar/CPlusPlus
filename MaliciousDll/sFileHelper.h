#include <iostream>
#include <vector>
using namespace std;
class sFileHelper
{
public:
	static bool isDirectoryExist(const string &p_crefstrPath);

	static bool isFileExist(const string &p_crefstrFilePath);

	static void getFolders(const string &p_crefstrPath,
		vector<string> &p_refvtstrFolders,
		const string &p_crefstrContains = "");

	static void getFoldersR(const string &p_crefstrPath,
		vector<string> &p_refvtstrFolders,
		const string &p_crefstrContains = "");

	static void getFiles(const string &p_crefstrPath,
		vector<string> &p_refvtstrFiles,
		const string & p_crefstrContains = "");

	static void getFilesR(const string &p_crefstrPath,
		vector<string> &p_refvtstrFiles,
		const string & p_crefstrContains = "");

	static void getLogicalDrives(vector<string> &p_refvtstrLogicalDrives);

	/*This function is responsible for creating folder recursively*/
	static bool createFolder(const string & p_crefstrFolder);

	static void appendTextInFile(const string& p_crefstrFilePath, const string &p_crefstrText);

	static bool deleteFile(const string &p_crefstrFilePath);
	static bool deleteFile(const string &p_crefstrFilePath, const string &p_crefstrFileName);

	static bool deleteFolder(const string &p_crefstrFolderPath);

	/*Responsible for copying input file, override if exist
	*/
	static void copyFile(const string &p_crefstrFromFilePath, const string &p_crefstrToFilePath);

	/*Responsible for copying folder in recursively (Internally call copyFile)
	*/
	static void copyFolder(const string &p_crefstrFromFolderPath, const string &p_crefstrToFolderPath);
};