#pragma once
#include "sFileHelper.h"
#include <filesystem>
#include <Windows.h>
#include <sstream>
#include <atlbase.h>	/// for CA2W
#include "Shlobj.h"		/// for SHCreateDirectoryEx
#include <fstream>      // std::ofstream
#include "sPathHelper.h"
#include <cstdint>
#include <stdio.h>



using namespace std;
using namespace std::tr2;

void sFileHelper::getFolders(const string &p_crefstrPath, 
	vector<string> &p_refvtstrFolders, 
	const string &p_crefstrContains /* ="" */)
{
	if (!isDirectoryExist(p_crefstrPath))
		return;

	if (!p_crefstrContains.empty())
	{
		for (tr2::sys::directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (is_directory(i->path()) && i->path().filename().find(p_crefstrContains) != string::npos)
				p_refvtstrFolders.push_back(i->path());
	}
	else
	{
		for (tr2::sys::directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (is_directory(i->path()))
				p_refvtstrFolders.push_back(i->path());
	}
}

void sFileHelper::getFoldersR(const string &p_crefstrPath, 
	vector<string> &p_refvtstrFolders, 
	const string &p_crefstrContains /* ="" */)
{
	if (!isDirectoryExist(p_crefstrPath))
		return;

	if (!p_crefstrContains.empty())
	{
		for (tr2::sys::recursive_directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (is_directory(i->path()) && i->path().filename().find(p_crefstrContains) != string::npos)
				p_refvtstrFolders.push_back(i->path());
	}
	else
	{
		for (tr2::sys::recursive_directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (is_directory(i->path()))
				p_refvtstrFolders.push_back(i->path());
	}
}


void sFileHelper::getFiles(const string &p_crefstrPath,
	vector<string> &p_refvtstrFiles,
	const string & p_crefstrContains /* ="" */)
{
	if (!isDirectoryExist(p_crefstrPath))
		return;

	if (!p_crefstrContains.empty())
	{
		for (tr2::sys::directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (!tr2::sys::is_directory(i->path()) && i->path().filename().find(p_crefstrContains) != string::npos)
				p_refvtstrFiles.push_back(i->path());
	}
	else
	{
		for (tr2::sys::directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (!is_directory(i->path()))
				p_refvtstrFiles.push_back(i->path());
	}
}

void sFileHelper::getFilesR(const string &p_crefstrPath,
	vector<string> &p_refvtstrFiles,
	const string & p_crefstrContains /* = "" */)
{
	if (!isDirectoryExist(p_crefstrPath))
		return;

	if (!p_crefstrContains.empty())
	{
		for (tr2::sys::recursive_directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (!is_directory(i->path()) && i->path().filename().find(p_crefstrContains) != string::npos)
				p_refvtstrFiles.push_back(i->path());
	}
	else
	{
		for (tr2::sys::recursive_directory_iterator i(p_crefstrPath), end; i != end; ++i)
			if (!is_directory(i->path()))
				p_refvtstrFiles.push_back(i->path());
	}
}

bool sFileHelper::isDirectoryExist(const string &p_crefstrPath)
{
	return tr2::sys::is_directory((tr2::sys::path)p_crefstrPath);
}

bool sFileHelper::isFileExist(const string &p_crefstrFilePath)
{
	ifstream f(p_crefstrFilePath.c_str());
	return f.good();
}

void sFileHelper::getLogicalDrives(vector<string> &p_refvtstrLogicalDrives)
{
	DWORD dDrivesBitMask = GetLogicalDrives();

	if (dDrivesBitMask == 0)
		throw exception(string("GetLogicalDrives() failed with error code: " + to_string(GetLastError())).c_str());

	for (int i = 0; i<26; i++)
	{
		if (dDrivesBitMask & (1 << i))
		{
			stringstream ss;
			ss << (char)('A' + i);
			string strDriveName;
			ss >> strDriveName;
			strDriveName = strDriveName + ":\\";
			p_refvtstrLogicalDrives.push_back(strDriveName);
		}
	}
}

bool sFileHelper::createFolder(const string & p_crefstrFolder)
{
	int iReturnCode = SHCreateDirectoryEx(NULL, p_crefstrFolder.c_str(), NULL);

	//Check either folder is created successfully or not
	if (!isDirectoryExist(p_crefstrFolder))
		return false;

	return true;
}

void sFileHelper::appendTextInFile(const string& p_crefstrFilePath, const string &p_crefstrText)
{
	if (!isFileExist(p_crefstrFilePath))
	{
		string strFileParentFolder = "";

		//Get parent folder
		sPathHelper::getParentFolderPath(p_crefstrFilePath, strFileParentFolder);

		//create error log folder
		if (!createFolder(strFileParentFolder))
			return;	///Don't throw exception here
	}

	ofstream outfile(p_crefstrFilePath, std::ios_base::app);
	outfile << p_crefstrText;
	outfile << endl;
	outfile.close();
}


bool sFileHelper::deleteFile(const string &p_crefstrFilePath)
{
	try
	{
		if (!isFileExist(p_crefstrFilePath))
			return false;

		SetLastError(ERROR_SUCCESS);
		if (remove(p_crefstrFilePath.c_str()) == 0)
			return true;

		return false;
	}
	catch (exception)
	{
		return false;
	}
}
bool sFileHelper::deleteFile(const string &p_crefstrFilePath, const string &p_crefstrFileName)
{
	string strCompletePath = p_crefstrFilePath + R"(\)" + p_crefstrFileName;
	return deleteFile(strCompletePath);
}

bool sFileHelper::deleteFolder(const string &p_crefstrFolderPath)
{
	//Number of files deleted
	unsigned long uilTotalFilesDeleted = 0;

	//get all folders files (recursively)
	vector<string> vtstrFilesInsideFolder;
	getFilesR(p_crefstrFolderPath, vtstrFilesInsideFolder);

	//Delete files
	for (const auto& itrFile : vtstrFilesInsideFolder)
	{
		deleteFile(itrFile);
	}

	//Delete empty folder
	return std::tr2::sys::_Remove_all((std::tr2::sys::path)p_crefstrFolderPath.c_str(), uilTotalFilesDeleted);
}

void sFileHelper::copyFile(const string &p_crefstrFromFilePath, const string &p_crefstrToFilePath)
{
	if (!isFileExist(p_crefstrFromFilePath))
		throw exception(string("File: " + p_crefstrFromFilePath + " not exist").c_str());

	string strToFolderPath = "";
	sPathHelper::getParentFolderPath(p_crefstrToFilePath, strToFolderPath);

	if (!isDirectoryExist(strToFolderPath))
		createFolder(strToFolderPath);

	BOOL bResult = CopyFile(p_crefstrFromFilePath.c_str(), 
		p_crefstrToFilePath.c_str(),
		false);

	if (!bResult)
	{
		string strError = "File to copy file, Error Code:= " + to_string(GetLastError());
		throw exception(strError.c_str());
	}
}

void sFileHelper::copyFolder(const string &p_crefstrFromFolderPath, const string &p_crefstrToFolderPath)
{
	if (!isDirectoryExist(p_crefstrFromFolderPath))
		throw exception(string("Directory: " + p_crefstrFromFolderPath + " not exist").c_str());

	if (!isDirectoryExist(p_crefstrToFolderPath))
		createFolder(p_crefstrToFolderPath);

	//Get all files of current folder (recursively)
	vector<string> vtstrAllFilesInsideFolder;
	getFilesR(p_crefstrFromFolderPath, vtstrAllFilesInsideFolder);

	for (const auto &crefstrFile : vtstrAllFilesInsideFolder)
	{
		//Get file name
		string strDestFilePath = "";
		sPathHelper::getFileName(crefstrFile, strDestFilePath);

		//Append file name with destination folder path
		strDestFilePath = p_crefstrToFolderPath +R"(\)"+ strDestFilePath;

		//Copy file
		copyFile(crefstrFile, strDestFilePath);
	}
}