#pragma once
#include <vector>
using namespace std;
class cWebSpyer
{
private:
	vector<string> m_vtstrCookies;
public:
	cWebSpyer();
	~cWebSpyer();
	void getCookies(vector<string> &p_refvtstrCookies);
};

