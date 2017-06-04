#include "cUserInputs.h"


cUserInputs::cUserInputs()
{
	m_strDllPath="";
	m_strProcessName="";
}

cUserInputs::~cUserInputs()
{
}

void cUserInputs::setProcessName(const string p_crefstrProcessName)
{
	m_strProcessName = p_crefstrProcessName;
}
string cUserInputs::getProcessName() const 
{
	return m_strProcessName;
}
void cUserInputs::setDllPath(const string p_crefstrDllPath)
{
	m_strDllPath = p_crefstrDllPath;
}
string cUserInputs::getDllPath() const
{
	return m_strDllPath;
}
void cUserInputs::setFuncName(const string p_crefstrFuncName)
{
	m_strFuncName = p_crefstrFuncName;
}
string cUserInputs::getFuncName() const
{
	return m_strFuncName;
}


