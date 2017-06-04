#include "cPackageCreator.h"
#include "cDataCompressor.h"
#include "sPathHelper.h"
#include "sConstants.h"

cPackageCreator::cPackageCreator()
{
}


cPackageCreator::~cPackageCreator()
{
}

void cPackageCreator::packageCookies(const vector<string> &p_crefvtstrDataFilesPath)
{
	string strCookiesDestPath = sConstants::getZipCookiesPath();
	createPackage(strCookiesDestPath, p_crefvtstrDataFilesPath);
}

void cPackageCreator::createPackage(const string &p_crefstrFilePath, const vector<string> &p_crefvtstrDataFilesPath)
{
	cDataCompressor dataCompressor(p_crefstrFilePath, p_crefvtstrDataFilesPath);
	dataCompressor.compress(true);
}

