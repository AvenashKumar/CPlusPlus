#include "cUserInputs.h"


cUserInputs::cUserInputs()
{
	m_strProcessName = "";
	m_bIsCreateNewProcess = false;
	m_bIsDefendProcess = false;
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

bool cUserInputs::isCreateNewProcess()const
{
	return m_bIsCreateNewProcess;
}
void cUserInputs::setCreateProcess(const bool p_cbIsCreateProcess/*=true*/)
{
	m_bIsCreateNewProcess = p_cbIsCreateProcess;
}
bool cUserInputs::isDefendProcess()const
{
	return m_bIsDefendProcess;
}
void cUserInputs::setDefendProcess(const bool p_cbIsDefendProcess/*=true*/)
{
	m_bIsDefendProcess = p_cbIsDefendProcess;
}
