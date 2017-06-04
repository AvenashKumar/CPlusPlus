#include "sFileHelper.h"
#include <fstream>
bool sFileHelper::isFileExist(const string &p_crefstrFilePath)
{
	ifstream f(p_crefstrFilePath.c_str());
	return f.good();
}