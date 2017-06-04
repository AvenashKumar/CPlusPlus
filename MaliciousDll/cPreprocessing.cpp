#include "cPreprocessing.h"
#include <iostream>
#include "sPathHelper.h"
#include "sFileHelper.h"
#include "sConstants.h"
using namespace std;

cPreprocessing::cPreprocessing()
{
}


cPreprocessing::~cPreprocessing()
{
}

void cPreprocessing::process()
{
	//Delete destination folder (If exist previously)
	deleteDestinationFolder();

	//Create destination folder (folder where zip will resides)
	createDestZipFolderAndDeletePreviousFile();
}

void cPreprocessing::deleteDestinationFolder()
{
	string strDestFolderPath = sConstants::PATH_DEST;
	sFileHelper::deleteFolder(strDestFolderPath);
}

void cPreprocessing::createDestZipFolderAndDeletePreviousFile()
{
	//Create destination folder, if it is not created.
	string strParentFolderPath = sConstants::PATH_DEST;
	sFileHelper::createFolder(strParentFolderPath);

	//Delete file if exist
	sFileHelper::deleteFile(sConstants::getZipCookiesPath());
}