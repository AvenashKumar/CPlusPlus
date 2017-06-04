#include "cMessages.h"
#include "sConstants.h"
#include <iostream>
cMessages::cMessages()
{
}


cMessages::~cMessages()
{
}

void cMessages::addErrorMessage(const string &p_crefstrErrorMsgs)
{
	m_vtstrErrorMsgs.push_back(sConstants::STR_ERROR + p_crefstrErrorMsgs);
}
void cMessages::addWarningMessage(const string &p_crefstrWarningMsgs)
{
	m_vtstrWarningMsgs.push_back(sConstants::STR_WARNING + p_crefstrWarningMsgs);
}

void cMessages::getMessages(vector<string>&p_refvtstrMessages)const
{
	p_refvtstrMessages.reserve(m_vtstrErrorMsgs.size() + m_vtstrWarningMsgs.size());
	p_refvtstrMessages.insert(p_refvtstrMessages.end(), m_vtstrErrorMsgs.begin(), m_vtstrErrorMsgs.end());
	p_refvtstrMessages.insert(p_refvtstrMessages.end(), m_vtstrWarningMsgs.begin(), m_vtstrWarningMsgs.end());
}

bool cMessages::isErrorFound() const
{
	return m_vtstrErrorMsgs.size() > 0;
}
void cMessages::printMessages()const
{
	vector<string> vtstrMessages;
	getMessages(vtstrMessages);
	for (const string &crefstrMessage : vtstrMessages)
	{
		cout << crefstrMessage.c_str() << endl;
	}
}
