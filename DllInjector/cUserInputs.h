#pragma once
#include <string>
using namespace std;
class cUserInputs
{
private:
	string m_strDllPath;
	string m_strProcessName;
	string m_strFuncName;
public:
	cUserInputs();
	~cUserInputs();

	void setProcessName(const string p_crefstrProcessName);
	string getProcessName() const;
	void setDllPath(const string p_crefstrDllPath);
	string getDllPath() const;
	void setFuncName(const string p_crefstrFuncName);
	string getFuncName() const;
};

