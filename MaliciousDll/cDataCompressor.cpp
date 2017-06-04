#include "cDataCompressor.h"
#include "sFileHelper.h"
#include "sPathHelper.h"
#include "XZip.h"
#include <tchar.h>

cDataCompressor::cDataCompressor(const string &p_crefstrCompressDataPath,
	const vector<string>&p_crefvtstrFilesToBeCompressed):
	m_strDestZipPath(p_crefstrCompressDataPath),
	m_vtstrFilesToBeCompressed(p_crefvtstrFilesToBeCompressed)
{
}

void cDataCompressor::compress(const bool p_cbIsHide /*= false*/)
{
	try
	{
		Zip(m_strDestZipPath, m_vtstrFilesToBeCompressed);

		//Hide zip file
		if (p_cbIsHide)
			SetFileAttributes(m_strDestZipPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
	}
	catch (const exception &e)
	{
		string strErrorMessage = "Exception File: cDataCompressor\nFunction: compress\nMessage: " + string(e.what());
		throw new exception(strErrorMessage.c_str());
	}
}

void cDataCompressor::Zip(const string &p_crefstrDestZipPath, const vector<string> &p_crefvtstrSrcFile)
{
	bool bResult = true;

	_ASSERTE(!p_crefstrDestZipPath.empty());

	HZIP hz = CreateZip((void *)p_crefstrDestZipPath.c_str(), 0, ZIP_FILENAME);

	try
	{
		if (hz)
		{
			for (const auto &itr : p_crefvtstrSrcFile)
			{
				// does zip source file exist?
				if (!sFileHelper::isFileExist(itr))
				{
					CloseZip(hz);
					string strError = "Zip source file " + itr + " cannot be found, operation aborted";
					throw exception(strError.c_str());
				}

				//get filename
				string strFileName = "";
				sPathHelper::getFileName(itr, strFileName);

				//Get dest zip file's parent folder path
				string strDestFolderPath = "";
				sPathHelper::getParentFolderPath(p_crefstrDestZipPath, strDestFolderPath);

				//Temp File Path (Copy: To)
				string strTempFile = strDestFolderPath + R"(\)" + strFileName;

				//Copy file that exist in itr(Copy: From) in strDestFolderPath
				sFileHelper::copyFile(itr, strTempFile);

				//Add temp file in zip folder
				if (ZipAdd(hz, strFileName.c_str(),
					(void *)strTempFile.c_str(),
					0,
					ZIP_FILENAME) == ZR_OK)
				{
					//Delete temp file 
					sFileHelper::deleteFile(strTempFile);
				}
			}

		}
		else
		{
			CloseZip(hz);
			string strError = "Failed to create zip file " + p_crefstrDestZipPath;
			throw exception(strError.c_str());
		}

		CloseZip(hz);
	}
	catch (exception e)
	{
		CloseZip(hz);
		throw exception(e.what());
	}
}