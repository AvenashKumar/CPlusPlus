#pragma once
#include <iostream>
#include <vector>
using namespace std;

class IBrowser
{
protected:
	const string m_cstrCookiesPath;
private:
	IBrowser();
public:
	IBrowser(const string &p_crefstrCookiesPath) :m_cstrCookiesPath(p_crefstrCookiesPath){}
public:
	virtual void searchCookies(vector<string> &p_refvtstrCookiesPath)=0;
};