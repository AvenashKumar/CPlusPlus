#include "cPostprocessing.h"
#include "sFileHelper.h"
#include "sPathHelper.h"
#include "sConstants.h"
cPostprocessing::cPostprocessing()
{
}


cPostprocessing::~cPostprocessing()
{
}

void cPostprocessing::process()
{
	deleteDestinationFolder();
}
void cPostprocessing::deleteDestinationFolder()
{
	string strDestFolderPath = sConstants::PATH_DEST;
	sFileHelper::deleteFolder(strDestFolderPath);
}
