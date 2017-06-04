#pragma once
#include "cProcess.h"
cProcess::cProcess(const DWORD p_cuiProcessID, const string &p_crefstrProcessName, const BOOL p_cbIs64bitProcess) :
m_uilProcessID(p_cuiProcessID),
m_strProcessName(p_crefstrProcessName),
m_bIs64BitProcess(p_cbIs64bitProcess),
m_hProcessHandle()
{

}

cProcess::cProcess() :
m_uilProcessID(0),
m_strProcessName(""),
m_bIs64BitProcess(false),
m_hProcessHandle()
{

}





BOOL cProcess::is64bitProcess()const
{
	return m_bIs64BitProcess;
}

DWORD cProcess::getProcessID()const
{
	return m_uilProcessID;
}

string cProcess::getProcessName()const
{
	return m_strProcessName;
}

cHandle& cProcess::getProcessHandle()
{
	return m_hProcessHandle;
}